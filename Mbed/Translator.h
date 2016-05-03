#include "stdafx.h"

#pragma once

// Translator class translates a raw message to a MessageInfo structure and back
class Translator {
public:
	// All info the Command info should contain
	struct MessageInfo {
		enum ECommandType { NONE, READ, WRITE, OPTION } CommandType;
		int Channel;
		int DAC;
	};

	struct Rules {
		static const int8_t StartChar = '!';
		static const int ReadWriteIndex = 1;
		static const int8_t ReadChar = 'r';
		static const int8_t WriteChar = 'w';
		static const int ChannelIndex = 2;
		static const int DataIndex = 3;
		static const int DataLength = 2;
		static const int Length = 4 + DataLength;
		static const int8_t EndChar = '?';

		static const int ReadCmdSize = 4;
		static const int WriteCmdSize = ReadCmdSize + DataLength;
	};

private: // Constructor private, impossible to make object of this static class
	Translator();
	~Translator();

public:
	static bool Translate(MessageInfo info, int8_t** const cmd);
	static bool Translate(MessageInfo info, std::string* const msg);
	static bool Translate(const int8_t* cmd, int cmdSize, MessageInfo* info);
};

