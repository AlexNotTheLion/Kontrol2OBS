#pragma once

#include <QtCore/QObject>

#include <obs.hpp>
#if __has_include(<obs-frontend-aph.h>)
#include <obs-frontend-api.h>
#else
#include <obs-frontend-api/obs-frontend-api.h>
#endif

class Events : public QObject{

	Q_OBJECT;

public:
	explicit Events();
	~Events() override;
	void startup();
	void shutdown();

	void connectSourceSignals(obs_source_t *source);
	void disconnectSourceSignals(obs_source_t *source);

private:
	void BroadcastCustomMessage(const char *name, bool state);

	static void OnSourceMuteChange(void *param, calldata_t *data);
	static void OnSourceVolumeChange(void *param, calldata_t *data);

	static void OnSourceCreate(void *param, calldata_t *data);
	static void OnSourceDestroy(void *param, calldata_t *data);

	static void FrontendEventHandler(enum obs_frontend_event event, void *privateData);


};
