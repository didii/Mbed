#include "stdafx.h"

#pragma once
#include "SerialPort.h"
#include "Translator.h"


class Listener {
private:
	SerialPort _port;

	Translator::MessageInfo _info;
	bool _isNewMessageReceived;

	static const int maxBufferSize = 256+2;
	int8_t _buffer[maxBufferSize];
	int _currIndex;

public:
	Listener();
	Listener(SerialPort port);

	bool Listen();
	bool Listen(clock_t timeOutMs);

	Translator::MessageInfo GetLastMessageInfo() const;

private:
	bool Add(int8_t buffer);
	bool Add(const int8_t* const buffer, DWORD charsToAdd);

	bool DoListen();

	bool IsNewMessageReceived();
};
