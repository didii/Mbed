#include "stdafx.h"

#pragma once
#include "Utility.h"


// Translator class translates a raw message to a MessageInfo structure and back
class Translator {
public:
	// All info the Command info should contain
	struct MessageInfo {
		enum ECommandType { NONE, READ, WRITE, OPTION, ERR } CommandType;
		int Channel;
		int DacValue;
		std::string ToString() const;
	};

	// Rules over how the command is structured
	struct Rules {
		static int    StartCharIndex;
		static int8_t StartChar;
		static int    CharsToReadIndex;
		static int    CharsToReadOffset;
		static int    rwoIndex;
		static int8_t ReadChar;
		static int8_t WriteChar;
		static int8_t OptionChar;
		static int8_t ErrorChar;
		//static int    ChannelIndex;
		static int    DataIndex;
		static int    DataLength;

		static int ReadCmdSize;
		static int WriteCmdSize;
		static int ErrorCmdSize;
	};

private:
	// Private constructor, cannot create object
	Translator();
	~Translator();

public:
	// Translate MessageInfo to raw int8_t* (NULL terminated!)
	static bool Translate(MessageInfo info, int8_t** const cmd, int* const cmdSize);
	// Translate raw int8_t* message to MessageInfo
	static void Translate(const int8_t* const cmd, int cmdSize, MessageInfo* const info);
};

// Only used in Translate(const int8_t* const,int,MessageInfo*)
class TranslatorException : public std::exception {
public:
	TranslatorException();
	TranslatorException(const char* const msg);
	TranslatorException(const char* const msg, int value);
};

class TranslatorStartCharMissingException : public TranslatorException {};
class TranslatorIncorrectSizeException : public TranslatorException {};
class TranslatorMessageTooShortException : public TranslatorIncorrectSizeException {};
class TranslatorMessageTooLongException : public TranslatorIncorrectSizeException {};