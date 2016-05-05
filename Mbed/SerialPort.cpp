#include "stdafx.h"
#include "SerialPort.h"
#include "Utility.h"

SerialPort::SerialPort()
	: _handle(INVALID_HANDLE_VALUE) {}

SerialPort::SerialPort(LPCWSTR portName)
	: _handle(INVALID_HANDLE_VALUE) {
	OpenPort(portName);
}

SerialPort::~SerialPort() {
	Close();
}

DWORD SerialPort::OpenPort(std::wstring portName) {
	// Make sure a _port wasn't already open
	if (_handle != INVALID_HANDLE_VALUE)
		throw "Close the _port before opening a new one";

	// Open the _port
	_handle = CreateFileW(portName.c_str(),
						  GENERIC_READ | GENERIC_WRITE,
						  0,
		                  nullptr,
		                  OPEN_EXISTING,
		                  FILE_FLAG_OVERLAPPED,
		                  nullptr);
	// Check if the _port was opened correctly
	if (_handle == INVALID_HANDLE_VALUE)
		return GetLastError();
	return 0;
}

bool SerialPort::IsOpen() const {
	return _handle != INVALID_HANDLE_VALUE;
}

DWORD SerialPort::ReadExisting(int8_t* c) const {
	// Get Comm state
	DWORD errors;
	COMSTAT stat;
	if (!ClearCommError(_handle, &errors, &stat))
		return GetLastError();

	// If no characters are waiting
	if (stat.cbInQue == 0)
		return -1; // bail

	// Create the overlapped event. Must be closed before exiting to avoid a handle leak.
	OVERLAPPED osReader = { 0 };
	osReader.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	if (osReader.hEvent == nullptr)
		return GetLastError();

	// Issue read for 1 character
	DWORD charsRead;
	if (!ReadFile(_handle, c, 1, &charsRead, &osReader))
		return GetLastError();

	// Report success
	return 0;
}

DWORD SerialPort::Read(int8_t** const msg, const DWORD charsToRead) const {
	DWORD charsRead;
	return Read(msg, charsToRead, &charsRead);
}

DWORD SerialPort::Read(int8_t** const msg, const DWORD charsToRead, DWORD* const charsRead) const {
	// Create the overlapped event. Must be closed before exiting to avoid a handle leak.
	OVERLAPPED osReader = { 0 };
	osReader.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	if (osReader.hEvent == nullptr)
		return GetLastError();

	// Issue read
	DWORD toRead = min(maxRead, charsToRead);
	int8_t* buffer = new int8_t[toRead];
	if (!ReadFile(_handle, buffer, toRead, charsRead, &osReader))
		return GetLastError();

	// Copy to msg
	DeepCopy(buffer, toRead, msg);

	return 0;
}

DWORD SerialPort::Read(std::string* const msg, const DWORD charsToRead) const {
	DWORD charsRead;
	return Read(msg, charsToRead, &charsRead);
}

DWORD SerialPort::Read(std::string* const msg, const DWORD charsToRead, DWORD* const charsRead) const {
	// Create the overlapped event. Must be closed before exiting to avoid a handle leak.
	OVERLAPPED osReader = { 0 };
	osReader.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	if (osReader.hEvent == nullptr)
		return GetLastError();

	// Issue read
	char buffer[maxRead];
	if (!ReadFile(_handle, buffer, min(maxRead, charsToRead), charsRead, &osReader))
		return GetLastError();

	// Copy buffer to string
	*msg = std::string(buffer, *charsRead);

	return 0;
}

DWORD SerialPort::ReadExisting(int8_t** const msg, DWORD* charsRead) const {
	// Get Comm state
	DWORD errors;
	COMSTAT stat;
	if (!ClearCommError(_handle, &errors, &stat))
		return GetLastError();

	// If no characters are waiting
	if (stat.cbInQue == 0) {
		return 0;
	}

	// Create this read operation's OVERLAPPED structure's hEvent.
	OVERLAPPED osReader = { 0 };
	osReader.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	if (osReader.hEvent == nullptr) {
		return GetLastError();
	}

	// Issue read
	DWORD toRead = min(maxRead, stat.cbInQue);
	int8_t* buffer = new int8_t[toRead];
	if (!ReadFile(_handle, buffer, toRead, charsRead, &osReader))
		return GetLastError();

	// Copy buffer to msg
	DeepCopy(buffer, *charsRead, msg);
	// Report success
	return 0;
}

DWORD SerialPort::ReadExisting(std::string*const msg) const {
	// Get Comm state
	DWORD errors;
	COMSTAT stat;
	if (!ClearCommError(_handle, &errors, &stat))
		return GetLastError();

	// If no characters are waiting
	if (stat.cbInQue == 0) {
		*msg = "";
		return 0;
	}

	// Create this read operation's OVERLAPPED structure's hEvent.
	OVERLAPPED osReader = { 0 };
	osReader.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	if (osReader.hEvent == nullptr) {
		return GetLastError();
	}

	// Issue read
	DWORD charsRead;
	char buffer[maxRead];
	if (!ReadFile(_handle, buffer, min(maxRead, stat.cbInQue), &charsRead, &osReader))
		return GetLastError();

	// Copy buffer to msg
	*msg = std::string(buffer, charsRead);
	
	// Report success
	return 0;
}

DWORD SerialPort::Write(const int8_t* const msg, const DWORD charsToWrite) const {
	DWORD charsWritten;
	return Write(msg, charsToWrite, &charsWritten);
}

DWORD SerialPort::Write(const int8_t* const msg, const DWORD charsToWrite, DWORD* const charsWritten) const {
	// Create this write operation's OVERLAPPED structure's hEvent.
	OVERLAPPED osWrite = { 0 };
	osWrite.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	if (osWrite.hEvent == nullptr)
		return GetLastError();

	// Issue write
	if (!WriteFile(_handle, msg, charsToWrite, charsWritten, &osWrite))
		return GetLastError();

	// Report success
	return 0;
}

DWORD SerialPort::Write(std::string msg) const {
	DWORD charsWritten;
	return Write(msg, &charsWritten);
}

DWORD SerialPort::Write(std::string msg, DWORD* const charsWritten) const {
	// Create this write operation's OVERLAPPED structure's hEvent.
	OVERLAPPED osWrite = { 0 };
	osWrite.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	if (osWrite.hEvent == nullptr)
		return GetLastError();

	// Issue write
	const char* buffer = msg.c_str();
	DWORD charsToWrite = msg.length();
	if (!WriteFile(_handle, buffer, charsToWrite, charsWritten, &osWrite))
		return GetLastError();

	// Report success
	return 0;
}

DWORD SerialPort::Close() {
	// Issue close handle
	if (!CloseHandle(_handle))
		return GetLastError();
	// Make sure it is invalidated
	_handle = INVALID_HANDLE_VALUE;
	// Report success
	return 0;
}

