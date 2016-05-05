#include "stdafx.h"

#pragma once

class SerialPort {
public:
	struct CommStruct {
		int readWriteIndex = 0;
		int channelIndex = 1;
		int dataIndex = 2;
		int dataLength = 2;
	private:
		unsigned char* command;

	public:
		// Constructor
		CommStruct();
		~CommStruct();

		// Setters
		void Clear() const;
		void SetRead() const;
		void SetWrite() const;
		void SetChannel(int ch) const;
		void SetData(int16_t data) const;

		// Getters
		bool IsRead() const;
		bool IsWrite() const;
		int GetChannel() const;
		unsigned char* GetData() const;
		int GetLength() const;
		std::string GetReadableCommand() const;

		friend class SerialPort;
	};

private:
	HANDLE _handle;
	CommStruct _commStruct;
	static const DWORD maxRead = 1024;

public:
	SerialPort();
	SerialPort(LPCWSTR portName);
	~SerialPort();

	DWORD OpenPort(std::wstring portName);
	DWORD Read(int8_t* c) const;
	DWORD Read(int8_t** const msg, const DWORD charsToRead) const;
	DWORD Read(int8_t** const msg, const DWORD charsToRead, DWORD* const charsRead) const;
	DWORD Read(std::string* const msg, const DWORD charsToRead) const;
	DWORD Read(std::string* const msg, const DWORD charsToRead, DWORD* const charsRead) const;
	DWORD ReadExisting(int8_t** const buffer, DWORD* charsRead) const;
	DWORD ReadExisting(std::string* const buffer) const;
	DWORD Write(const int8_t* const msg, const DWORD charsToWrite) const;
	DWORD Write(const int8_t* const msg, const DWORD charsToWrite, DWORD* const charsWritten) const;
	DWORD Write(std::string msg) const;
	DWORD Write(std::string msg, DWORD* const charsWritten) const;
	DWORD Close();
};

