#include "stdafx.h"
#include "Listener.h"

Listener::Listener()
	: _port(),
	_info(),
	_isNewMessageReceived(false),
	_buffer(),
	_currIndex(0) {}

Listener::Listener(SerialPort port)
	: _port(port),
	_info(),
	_isNewMessageReceived(false),
	_buffer(),
	_currIndex(0) {}

bool Listener::Listen() {
	while (true) {
		if (DoListen())
			return true;
		Sleep(5);
	}
}

bool Listener::Listen(clock_t timeOutMs) {
	clock_t startTime = clock();
	while (true) {
		if (DoListen())
			return true;
		if ((clock() - startTime) / CLOCKS_PER_SEC > timeOutMs)
			return false;
		Sleep(5);
	}
}

Translator::MessageInfo Listener::GetLastMessageInfo() const {
	return _info;
}

bool Listener::Add(int8_t c) {
	// Add character to buffer
	_buffer[_currIndex] = c;
	// Check if valid message by translating it
	Translator::MessageInfo info;
	try {
		Translator::Translate(_buffer, _currIndex + 1, &info);
	}
	catch (TranslatorStartCharMissingException) {
		// Faulty message: reset
		_currIndex = 0;
		return false;
	}
	catch (TranslatorMessageTooLongException) {
		// Faulty message: reset
		_currIndex = 0;
		return false;
	}
	catch (TranslatorMessageTooShortException) {
		// Message too short = incomplete, wait for other chars
		return false;
	}
	// Translation succesful, copy to _info
	_info = info;
	// Notify user of new message
	_isNewMessageReceived = true;
	// Reset counter
	_currIndex = 0;
	// Report success
	return true;
}

bool Listener::Add(const int8_t* const buffer, DWORD charsToAdd) {
	bool result = false;
	for (int i = 0; i < charsToAdd; i++, _currIndex++) {
		if (Add(buffer[i]))
			result = true;
	}
	return result;
}

bool Listener::DoListen() {
	int8_t c;
	if (_port.Read(&c) != 0)
		return false;

	return Add(c);
}

bool Listener::IsNewMessageReceived() {
	if (!_isNewMessageReceived)
		return false;
	_isNewMessageReceived = false;
	return true;
}
