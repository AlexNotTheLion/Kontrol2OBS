#pragma once

#include <QtCore/QObject>

#if __has_include(<obs-frontend-api.h>)
#include <obs-frontend-api.h>
#else
#include <obs-frontend-api/obs-frontend-api.h>
#endif

#include "kontrol2obs.h"

class OBSController
{
public:
	OBSController(const char * name, bool state);
	~OBSController();

	void sendMessage();
};
