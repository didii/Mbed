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
	DWORD Read(std::string& msg, DWORD charsToRead) const;
	DWORD Read(std::string& msg, DWORD charsToRead, DWORD& charsRead) const;
	DWORD ReadExisiting(std::string& buffer) const;
	DWORD Write(std::string msg) const;
	DWORD Write(std::string msg, DWORD& charsWritten) const;
	DWORD Close();
};

