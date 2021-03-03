#include "events.h"
#include <utility>

#include "forms/settings-dialog.h"

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
const char *calldata_get_string(const calldata_t *data, const char *name)
{
	const char *value = nullptr;
	calldata_get_string(data, name, &value);
	return value;
}

Events::Events()
{
	//make a nanoKontrol.cpp and header in main, rename plugin-main to obs-kontrol.cpp and h
	//pass reference of nanoKontrol here
	//call events when they happen

	signal_handler_t *coreSignalHandler = obs_get_signal_handler();
	if (coreSignalHandler) {
		signal_handler_connect(coreSignalHandler, "source_create", OnSourceCreate, this);
		signal_handler_connect(coreSignalHandler, "source_destroy", OnSourceDestroy, this);
	}
}


Events::~Events()
{
	signal_handler_t *coreSignalHandler = obs_get_signal_handler();
	if (coreSignalHandler) {
		signal_handler_disconnect(coreSignalHandler, "source_destroy", OnSourceDestroy, this);
		signal_handler_disconnect(coreSignalHandler, "source_create", OnSourceCreate, this);
	}
}

void Events::connectSourceSignals(obs_source_t *source)
{
	blog(LOG_DEBUG, "kontrol - connected source");
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
	blog(LOG_DEBUG, "kontrol - audio source mute changed");
}

void Events::OnSourceVolumeChange(void *param, calldata_t *data)
{
	blog(LOG_DEBUG, "kontrol - audio source volume changed");
}
