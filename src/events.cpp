#include "events.h"
#include <utility>
#include <string>
#include <stdio.h>
#include "forms/settings-dialog.h"
#include "obs-controller.h"

const char *sourceTypeToString(obs_source_type type)
{
	switch (type) {
	case OBS_SOURCE_TYPE_INPUT:
		return "input";
	case OBS_SOURCE_TYPE_SCENE:
		return "scene";
	case OBS_SOURCE_TYPE_TRANSITION:
		return "transition";
	case OBS_SOURCE_TYPE_FILTER:
		return "filter";
	default:
		return "unknown";
	}
}

template<typename T>
T *calldata_get_pointer(const calldata_t *data, const char *name)
{
	void *ptr = nullptr;
	calldata_get_ptr(data, name, &ptr);
	return reinterpret_cast<T *>(ptr);
}

Events::Events()
{
	this->setParent(plugin_window);
	obs_frontend_add_event_callback(Events::FrontendEventHandler, this);
}


Events::~Events()
{
}

void Events::startup()
{
	obs_enum_sources(
		[](void *param, obs_source_t *source) {
			auto self = reinterpret_cast<Events *>(param);
			self->connectSourceSignals(source);
			return true;
		},
		this);

	signal_handler_t *coreSignalHandler = obs_get_signal_handler();
	if (coreSignalHandler) {
		signal_handler_connect(coreSignalHandler, "source_create",
					OnSourceCreate, this);
		signal_handler_connect(coreSignalHandler, "source_destroy",
					OnSourceDestroy, this);
	}
}

void Events::shutdown()
{
	signal_handler_t *coreSignalHandler = obs_get_signal_handler();
	if (coreSignalHandler) {
		signal_handler_disconnect(coreSignalHandler, "source_destroy",
						OnSourceDestroy, this);
		signal_handler_disconnect(coreSignalHandler, "source_create",
						OnSourceCreate, this);
	}

	obs_enum_sources(
		[](void *param, obs_source_t *source) {
			auto self = reinterpret_cast<Events *>(param);
			self->disconnectSourceSignals(source);
			return true;
		},
		this);
	obs_frontend_remove_event_callback(Events::FrontendEventHandler, this);
}

void Events::BroadcastCustomMessage(const char *name, bool state)
{
	blog(LOG_DEBUG, "kontrol - setting source %s to %s", name, state ? "true" : "false");

	obs_source *src = obs_get_source_by_name(name);
	if (!src) {
		throw("sourceName not found");
	}

	if (obs_source_muted(src) != state) {
		obs_source_set_muted(src, state);
		return;
	}
}


void Events::connectSourceSignals(obs_source_t *source)
{
	if (!source) {
		return;
	}
	// Disconnect everything first to avoid double-binding
	disconnectSourceSignals(source);
	signal_handler_t *sh = obs_source_get_signal_handler(source);
	signal_handler_connect(sh, "mute", OnSourceMuteChange, this);
	signal_handler_connect(sh, "volume", OnSourceVolumeChange, this);

}

void Events::disconnectSourceSignals(obs_source_t *source)
{
	if (!source) {
		return;
	}
	signal_handler_t *sh = obs_source_get_signal_handler(source);
	signal_handler_disconnect(sh, "mute", OnSourceMuteChange, this);
	signal_handler_disconnect(sh, "volume", OnSourceVolumeChange, this);
}

void Events::FrontendEventHandler(enum obs_frontend_event event, void *private_data)
{
	auto owner = reinterpret_cast<Events *>(private_data);

	if (event == OBS_FRONTEND_EVENT_FINISHED_LOADING)
	{
		owner->startup();
	}
}



void Events::OnSourceCreate(void *param, calldata_t *data)
{
	auto self = reinterpret_cast<Events *>(param);
	OBSSource source = calldata_get_pointer<obs_source_t>(data, "source");
	if (!source) {
		return;
	}
	self->connectSourceSignals(source);
}

void Events::OnSourceDestroy(void *param, calldata_t *data)
{
	auto self = reinterpret_cast<Events *>(param);
	obs_source_t *source = calldata_get_pointer<obs_source_t>(data, "source");
	if (!source) {
		return;
	}
	self->disconnectSourceSignals(source);
	
}

void Events::OnSourceMuteChange(void *param, calldata_t *data)
{
	auto self = reinterpret_cast<Events *>(param);
	OBSSource source = calldata_get_pointer<obs_source_t>(data, "source");
	if (!source) {
		return;
	}
	bool muted = false;
	if (!calldata_get_bool(data, "muted", &muted)) {
		return;
	}
	
	blog(LOG_DEBUG, "Kontrol - %s isMuted: %s", obs_source_get_name(source), muted ? "true" : "false");

	self->BroadcastCustomMessage("Mic/Aux 4", muted);

}

void Events::OnSourceVolumeChange(void *param, calldata_t *data)
{
	blog(LOG_DEBUG, "kontrol - audio source volume changed");
}

