#include "obs-controller.h"

OBSController::OBSController(const char* name, bool state)
{
	obs_source_t *src = obs_get_source_by_name(name);
	if (!src) {
		throw("sourceName not found");
		//fucking do some shit i dont know
	}

	obs_source_set_muted(src, state);
	obs_source_release(src);
}

OBSController::~OBSController() = default;

void OBSController::sendMessage()
{
	
}
