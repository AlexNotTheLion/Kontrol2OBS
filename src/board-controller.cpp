#include "board-controller.h"


BoardController::BoardController()
{
	setupBoard();
}


BoardController::~BoardController()
{
	shutdownBoard();
}


void BoardController::setupBoard()
{
	int in_portCount = _midiIn.get_port_count();
	int out_portCount = _midiOut.get_port_count();

	for (int i = 0; i < in_portCount; i++) {
		blog(LOG_DEBUG, "in port number %i, name:%s\n", i, _midiIn.get_port_name(i).c_str());
	}

	for (int i = 0; i < out_portCount; i++) {
		blog(LOG_DEBUG, "out port number %i, name:%s\n", i, _midiOut.get_port_name(i).c_str());
	}

	_midiIn.open_port(0, "nanoKONTROL2 1 SLIDER/KNOB 0");
	_midiOut.open_port(1, "nanoKONTROL2 1 CTRL 1");

	sendMessage({0xB0, 0x30, 0x7F});
}

void BoardController::shutdownBoard()
{
	_midiIn.close_port();
	_midiOut.close_port();
}

void BoardController::sendMessage(std::vector<unsigned char> message)
{
	_midiOut.send_message(message);
}
