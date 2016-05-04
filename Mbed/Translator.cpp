#include "stdafx.h"
#include "Translator.h"

int    Translator::Rules::StartCharIndex = 0;
int8_t Translator::Rules::StartChar = '!';
int    Translator::Rules::CharsToReadIndex = 1;
int    Translator::Rules::CharsToReadOffset = CharsToReadIndex+1;
int    Translator::Rules::rwoIndex = 2;
int8_t Translator::Rules::ReadChar = 'r';
int8_t Translator::Rules::WriteChar = 'w';
int8_t Translator::Rules::OptionChar = 'o';
//int    Translator::Rules::ChannelIndex = 3;
int    Translator::Rules::DataIndex = 3;
int    Translator::Rules::DataLength = 2;

int    Translator::Rules::ReadCmdSize = rwoIndex + 1;
int    Translator::Rules::WriteCmdSize = DataIndex + DataLength;


Translator::Translator() {}

Translator::~Translator() {}

bool Translator::Translate(MessageInfo info, int8_t** cmd, int* const cmdSize) {
	switch (info.CommandType) {
		case MessageInfo::NONE:
			return false;
		case MessageInfo::READ:
			*cmdSize = Rules::ReadCmdSize;
			*cmd = new int8_t[Rules::ReadCmdSize];
			(*cmd)[Rules::StartCharIndex] = Rules::StartChar;
			(*cmd)[Rules::CharsToReadIndex] = Rules::ReadCmdSize-Rules::CharsToReadOffset;
			(*cmd)[Rules::rwoIndex] = Rules::ReadChar;
			//(*cmd)[Rules::ChannelIndex] = info.Channel;
			break;
		case MessageInfo::WRITE:
			*cmdSize = Rules::WriteCmdSize;
			*cmd = new int8_t[Rules::WriteCmdSize];
			(*cmd)[Rules::StartCharIndex] = Rules::StartChar;
			(*cmd)[Rules::CharsToReadIndex] = Rules::WriteCmdSize - Rules::CharsToReadOffset;
			(*cmd)[Rules::rwoIndex] = Rules::WriteChar;
			//(*cmd)[Rules::ChannelIndex] = info.Channel;
			for (int i = 0; i < Rules::DataLength; i++)
				(*cmd)[Rules::DataIndex + i] = (info.DacValue >> (Rules::DataLength - i - 1) * 8) & 0xFF;
			break;
		case MessageInfo::OPTION:
			// Not implemented yet
			return false;
		default:
			return false;
	}

	return true;
}

bool Translator::Translate(MessageInfo info, std::string* const msg) {
	// Empty the message
	*msg = "";

	// Check the command type
	switch (info.CommandType) {
		case MessageInfo::NONE:
			return false; // report unsuccesful
		case MessageInfo::READ:
			*msg += 'r';
			*msg += std::to_string(info.Channel);
			break;
		case MessageInfo::WRITE:
			*msg += 'w';
			*msg += std::to_string(info.Channel);
			*msg += "(" + std::to_string(info.DacValue) + ")";
			break;
		case MessageInfo::OPTION:
			// Not implemented yet
			return false;
		default:
			return false;
	}
	return true;
}

bool Translator::Translate(const int8_t* const cmd, int cmdSize, MessageInfo* const info) {
	try {
		// Check starting character
		if (cmd[Rules::StartCharIndex] != Rules::StartChar)
			throw TranslatorException("No starting character present");
		// Check length of command
		if (cmd[Rules::CharsToReadIndex] + Rules::CharsToReadOffset != cmdSize)
			throw TranslatorException("Sizes do not match");

		// Populate info
		switch (cmd[Rules::rwoIndex]) {
		case 'r':
			info->CommandType = MessageInfo::READ;
			break;
		case 'w':
			info->CommandType = MessageInfo::WRITE;
			break;
		case 'o':
			info->CommandType = MessageInfo::OPTION;
			break;
		default:
			throw TranslatorException("Incorrect command type");
		}
		//info->Channel = cmd[Rules::ChannelIndex];
		info->DacValue = ByteUnshift(cmd+Rules::DataIndex, Rules::DataLength);
	}
	catch (TranslatorException e) {
		// Invalidate info
		info->CommandType = MessageInfo::NONE;
		info->Channel = 0;
		info->DacValue = 0;
		// Report unsuccesful translate
		return false;
	}
	return true;
}



TranslatorException::TranslatorException()
	: std::exception() {}

TranslatorException::TranslatorException(const char* const msg)
	: std::exception(msg) {}

TranslatorException::TranslatorException(const char* const msg, int value)
	: std::exception(msg, value) {}
