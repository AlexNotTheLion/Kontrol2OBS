#pragma once

#include <obs.hpp>
#if __has_include(<obs-frontend-aph.h>)
#include <obs-frontend-api.h>
#else
#include <obs-frontend-api/obs-frontend-api.h>
#endif

class Events {
public:
	explicit Events();
	~Events();

	void connectSourceSignals(obs_source_t *source);
	void disconnectSourceSignals(obs_source_t *source);

private:
	static void FrontendEventHandler(enum obs_frontend_event event,void *private_data);

	static void OnSourceAudioMixersChanged(void *param, calldata_t *data);
};
