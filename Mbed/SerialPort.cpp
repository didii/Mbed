#include "stdafx.h"
#include "SerialPort.h"

SerialPort::CommStruct::CommStruct() {
	command = new unsigned char[GetLength()];
}

SerialPort::CommStruct::~CommStruct() {
	delete command;
}

void SerialPort::CommStruct::Clear() const {
	command[readWriteIndex] = 0;
	command[channelIndex] = 0;
	for (int i = dataIndex; i < dataIndex + dataLength; i++)
		command[i] = 0;
}

void SerialPort::CommStruct::SetRead() const {
	command[readWriteIndex] = 'r';
}

void SerialPort::CommStruct::SetWrite() const {
	command[readWriteIndex] = 'w';
}

void SerialPort::CommStruct::SetChannel(int ch) const {
	command[channelIndex] = ch;
}

void SerialPort::CommStruct::SetData(int16_t data) const {
	for (int i = 0; i < dataLength; i++) {
		command[i + dataIndex] = (data >> (dataLength - i - 1) * 8) & 0xFF;
	}
}

bool SerialPort::CommStruct::IsRead() const {
	return command[readWriteIndex] == 'r';
}

bool SerialPort::CommStruct::IsWrite() const {
	return command[readWriteIndex] == 'w';
}

int SerialPort::CommStruct::GetChannel() const {
	return std::atoi((char*)(command + channelIndex));
}

unsigned char* SerialPort::CommStruct::GetData() const {
	return command + dataIndex;
}

int SerialPort::CommStruct::GetLength() const {
	return 2 + dataLength;
}

std::string SerialPort::CommStruct::GetReadableCommand() const {
	std::stringstream ss;
	ss << command[0] << (int)command[1] << "(" << (int)command[2] * 256 + (int)command[3] << ")";
	return ss.str();
}


SerialPort::SerialPort() {
	_handle = INVALID_HANDLE_VALUE;
}

SerialPort::SerialPort(LPCWSTR portName) {
	OpenPort(portName);
}

SerialPort::~SerialPort() {
}

DWORD SerialPort::OpenPort(std::wstring portName) {
	// Make sure a port wasn't already open
	if (_handle != INVALID_HANDLE_VALUE)
		throw "Close the port before opening a new one";

	// Open the port
	_handle = CreateFileW(portName.c_str(),
						  GENERIC_READ | GENERIC_WRITE,
						  0,
		                  nullptr,
		                  OPEN_EXISTING,
		                  FILE_FLAG_OVERLAPPED,
		                  nullptr);
	// Check if the port was opened correctly
	if (_handle == INVALID_HANDLE_VALUE)
		return GetLastError();
	return 0;
}

DWORD SerialPort::Read(std::string& msg, DWORD charsToRead) const {
	DWORD charsRead;
	return Read(msg, charsToRead, charsRead);
}

DWORD SerialPort::Read(std::string& msg, DWORD charsToRead, DWORD& charsRead) const {
	// Create the overlapped event. Must be closed before exiting to avoid a handle leak.
	OVERLAPPED osReader = { 0 };
	osReader.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	if (osReader.hEvent == nullptr)
		return GetLastError();

	char buffer[maxRead];
	if (!ReadFile(_handle, buffer, min(maxRead, charsToRead), &charsRead, &osReader))
		return GetLastError();
	msg = std::string(buffer, charsRead);

	return 0;
}

DWORD SerialPort::ReadExisiting(std::string& msg) const {
	// Get Comm state
	DWORD errors;
	COMSTAT stat;
	if (!ClearCommError(_handle, &errors, &stat))
		return GetLastError();

	// If no characters are waiting
	if (stat.cbInQue == 0) {
		msg = "";
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
	msg = std::string(buffer, charsRead);
	
	return 0;
}

DWORD SerialPort::Write(std::string msg) const {
	DWORD charsWritten;
	return Write(msg, charsWritten);
}

DWORD SerialPort::Write(std::string msg, DWORD& charsWritten) const {
	// Create this write operation's OVERLAPPED structure's hEvent.
	OVERLAPPED osWrite = { 0 };
	osWrite.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	if (osWrite.hEvent == nullptr)
		return GetLastError();

	// Issue write
	const char* buffer = msg.c_str();
	DWORD charsToWrite = msg.length();
	if (!WriteFile(_handle, buffer, charsToWrite, &charsWritten, &osWrite))
		return GetLastError();
	return 0;
}

DWORD SerialPort::Close() {
	if (!CloseHandle(_handle))
		return GetLastError();
	return 0;
}

