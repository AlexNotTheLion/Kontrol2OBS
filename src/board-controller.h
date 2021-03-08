#pragma once
#if __has_include(<obs-frontend-api.h>)
#include <obs-frontend-api.h>
#else
#include <obs-frontend-api/obs-frontend-api.h>
#endif

#include "RtMidi17/rtmidi17.hpp"

class BoardController {
public:
	BoardController();
	~BoardController();

	void sendMessage(std::vector<unsigned char> message);

private:
	rtmidi::midi_in _midiIn;
	rtmidi::midi_out _midiOut;

	void setupBoard();
	void shutdownBoard();
};
