#include "stdafx.h"

#pragma once

// Opens a serial port and allows easy reading and writing
// All functions return 0 for succesfull, any other number for an error
class SerialPort {
private:
	// The handle to the port
	HANDLE _handle;
	// Maximum bytes to read at once
	static const DWORD maxRead = 1024;

public:
	// Default constructor
	SerialPort();
	// Constructor which immediately opens a port
	SerialPort(std::wstring portName);
	// Default destructor
	~SerialPort();

	// Opens the given port. Cannot be used if a port was already opened
	DWORD OpenPort(std::wstring portName);
	// Check if the port is open
	bool IsOpen() const;

	// Read the given number of characters
	DWORD Read(int8_t** const msg, const DWORD charsToRead) const;
	// Read the given number of characters
	DWORD Read(int8_t** const msg, const DWORD charsToRead, DWORD* const charsRead) const;
	// Read the given number of characters and store it in a string
	DWORD Read(std::string* const msg, const DWORD charsToRead) const;
	// Read the given number of characters and store it in a string
	DWORD Read(std::string* const msg, const DWORD charsToRead, DWORD* const charsRead) const;
	// Amount of characters available to read
	DWORD CharactersWaiting() const;
	// Read a single character if it is available
	DWORD ReadExisting(int8_t* c) const;
	// Read all characters from the input buffer (max size = maxRead)
	DWORD ReadExisting(int8_t* const buffer, DWORD* charsRead) const;
	// Read all characters from the input buffer (max size = maxRead)
	DWORD ReadExisting(std::string* const buffer) const;

	// Write characters to the port
	DWORD Write(const int8_t* const msg, const DWORD charsToWrite) const;
	// Write characters to the port
	DWORD Write(const int8_t* const msg, const DWORD charsToWrite, DWORD* const charsWritten) const;
	// Write the complete string to the port
	DWORD Write(std::string msg) const;
	// Write the complete string to the port
	DWORD Write(std::string msg, DWORD* const charsWritten) const;

	// Close the port (used in destructor)
	DWORD Close();
};

