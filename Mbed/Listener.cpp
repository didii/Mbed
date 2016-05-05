#include "stdafx.h"
#include "Listener.h"

Listener::Listener()
	:_info(),
	_isNewMessageReceived(false),
	_buffer(),
	_currIndex(0) {}

bool Listener::Listen(const SerialPort& port) {
	// Start infinite loop
	while (true) {
		// Get a single character
		if (DoListen(port))
			return true; // New message was received
		// Wait a bit to reduce load
		Sleep(5);
	}
}

bool Listener::Listen(const SerialPort& port, clock_t timeOutMs) {
	// Save current time
	clock_t startTime = clock();
	// Start infinte loop
	while (true) {
		// Get a single character
		if (DoListen(port))
			return true; // New message was received
		// Check if time-out was overdue
		if ((clock() - startTime) / CLOCKS_PER_SEC > timeOutMs)
			return false; // Report unsuccess
		// Wait a bit to reduce load
		Sleep(5);
	}
}

Translator::MessageInfo Listener::GetLastMessageInfo() const {
	return _info;
}

bool Listener::Sync(const SerialPort& port) {
	// Read all existing characters
	std::string temp = "";
	if (port.ReadExisting(&temp) != 0)
		return false;
	// Reset index
	_currIndex = 0;
	return true;
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

bool Listener::DoListen(const SerialPort& port) {
	// Read a single character
	int8_t c;
	if (port.ReadExisting(&c) != 0)
		return false;
	// Add it to the buffer and check for a new message
	return Add(c);
}

bool Listener::IsNewMessageReceived() {
	// Returns true only once
	if (!_isNewMessageReceived)
		return false;
	_isNewMessageReceived = false;
	return true;
}
