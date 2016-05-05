#include "stdafx.h"

#pragma once
#include "Utility.h"


// Translator class translates a raw message to a MessageInfo structure and back
class Translator {
public:
	// All info the Command info should contain
	struct MessageInfo {
		// Type of command
		enum ECommandType { NONE, READ, WRITE, OPTION, ERR } CommandType;
		// Channel value (unused)
		int Channel;
		// DAC value
		int DacValue;
		
		// Set as readable string
		std::string ToString() const;
	};

	// Rules over how the command is structured
	struct Rules {
		
		static int    StartCharIndex;    // Location of starting char
		static int8_t StartChar;         // Starting character
		static int    CharsToReadIndex;  // Location of ToRead number
		static int    CharsToReadOffset; // Number of characters in prefix
		static int    CommandIndex; // Location of command type char
		static int8_t ReadChar;     // Character to indicate read command
		static int8_t WriteChar;    // Character to indicate write command
		static int8_t OptionChar;   // Character to indicate option command
		static int8_t ErrorChar;    // Character to indicate error command
		static int    ChannelIndex; // Location of channel char (unused)
		static int    DataIndex;    // Location of data start
		static int    DataLength;   // Number of chars used in data

		static int ReadCmdSize;  // Size of a read command
		static int WriteCmdSize; // Size of a write command
		static int ErrorCmdSize; // Size of an error command
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

// <Dit wordt gebruikt in de vermelde Translate functie. Zo kan Translate in een try block staan
//  en de exception opgevangen worden naar gelang welk type error er geweest is. Wordt gebruikt>
//  in Listener::Add>
// Only used in Translate, parent class of Translator exceptions
class TranslatorException : public std::exception {
public:
	TranslatorException(const int8_t* const cmd, const int cmdSize)
		: RawMessage(cmd), RawMessageSize(cmdSize) {}
	const int8_t* const RawMessage;
	const int RawMessageSize;
};

// Exception thrown when the first character of the buffer isn't Translator::Rules::StartChar
class TranslatorStartCharMissingException : public TranslatorException {
public:
	TranslatorStartCharMissingException(const int8_t* const cmd, const int cmdSize)
		: TranslatorException(cmd,cmdSize) {}
};

// Exception thrown when given command size does not correspond to charsToRead of the command itself
class TranslatorIncorrectSizeException : public TranslatorException {
public:
	TranslatorIncorrectSizeException(const int8_t* const cmd, const int cmdSize)
		: TranslatorException(cmd, cmdSize) {}
};
// Exception thrown when message is too short
// Catching TranslatorIncorrectSizeException will catch this as well
class TranslatorMessageTooShortException : public TranslatorIncorrectSizeException {
public:
	TranslatorMessageTooShortException(const int8_t* const cmd, const int cmdSize)
		: TranslatorIncorrectSizeException(cmd, cmdSize) {}
};
// Exception thrown when message is too long
// Catching TranslatorIncorrectSizeException will catch this as well
class TranslatorMessageTooLongException : public TranslatorIncorrectSizeException {
public:
	TranslatorMessageTooLongException(const int8_t* const cmd, const int cmdSize)
		: TranslatorIncorrectSizeException(cmd, cmdSize) {}
};

// Exception thrown when when command character isn't recognized
class TranslatorIncorrectCommandException : public TranslatorException {
public:
	TranslatorIncorrectCommandException(const int8_t* const cmd, const int cmdSize)
		: TranslatorException(cmd, cmdSize) {}
};