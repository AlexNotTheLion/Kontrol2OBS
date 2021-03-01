#include "events.h"

Events::Events()
{
	obs_frontend_add_event_callback(Events::FrontendEventHandler, this);

	obs_enum_sources(
		[](void *param, obs_source_t *source) {
			auto self = reinterpret_cast<Events *>(param);
			self->connectSourceSignals(source);
			return true;
		},
		this);
}


Events::~Events()
{
	obs_enum_sources(
		[](void *param, obs_source_t *source) {
			auto self = reinterpret_cast<Events *>(param);
			self->disconnectSourceSignals(source);
			return true;
		},
		this);
}


void Events::FrontendEventHandler(enum obs_frontend_event event, void *private_data)
{
	auto owner = reinterpret_cast<Events *>(private_data);

}


void Events::connectSourceSignals(obs_source_t *source)
{
	if (!source) {
		return;
	}
	// Disconnect everything first to avoid double-binding
	disconnectSourceSignals(source);
	obs_source_type sourceType = obs_source_get_type(source);
	signal_handler_t *sh = obs_source_get_signal_handler(source);
	signal_handler_connect(sh, "audio_mixers", OnSourceAudioMixersChanged,this);

}

void Events::disconnectSourceSignals(obs_source_t *source)
{
	if (!source) {
		return;
	}
	signal_handler_t *sh = obs_source_get_signal_handler(source);
	signal_handler_disconnect(sh, "audio_mixers",OnSourceAudioMixersChanged, this);
}

void Events::OnSourceAudioMixersChanged(void *param, calldata_t *data)
{
	blog(LOG_DEBUG, "audio source changed");
}
