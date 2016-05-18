#include "stdafx.h"
#include "Interactor.h"
#include "Listener.h"
#include "Translator.h"
#include "SerialPort.h"
#pragma once

class Communicator {
private:
	SerialPort _port;
	Interactor _interactor;
	Listener _listener;

public:

};