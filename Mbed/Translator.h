#include "stdafx.h"

#pragma once
#include "Utility.h"


// Translator class translates a raw message to a MessageInfo structure and back
class Translator {
public:
	// All info the Command info should contain
	struct MessageInfo {
		enum ECommandType { NONE, READ, WRITE, OPTION } CommandType;
		int Channel;
		int DacValue;
	};

	struct Rules {
		static int    StartCharIndex;
		static int8_t StartChar;
		static int    CharsToReadIndex;
		static int    CharsToReadOffset;
		static int    rwoIndex;
		static int8_t ReadChar;
		static int8_t WriteChar;
		static int8_t OptionChar;
		//static int    ChannelIndex;
		static int    DataIndex;
		static int    DataLength;

		static int ReadCmdSize;
		static int WriteCmdSize;
	};

private:
	// Private constructor, cannot create object
	Translator();
	~Translator();

public:
	// Translate MessageInfo to raw int8_t*
	static bool Translate(MessageInfo info, int8_t** const cmd, int* const cmdSize);
	// Translate MessageInfo to readable string
	static bool Translate(MessageInfo info, std::string* const msg);
	// Translate raw int8_t* message to MessageInfo
	static bool Translate(const int8_t* const cmd, int cmdSize, MessageInfo* const info);
};

// Only used in Translate(const int8_t* const,int,MessageInfo*)
class TranslatorException : public std::exception {
public:
	TranslatorException();
	TranslatorException(const char* const msg);
	TranslatorException(const char* const msg, int value);
};