#include "stdafx.h"
#include "Translator.h"

// Values for the rules
int    Translator::Rules::StartCharIndex = 0;
int8_t Translator::Rules::StartChar = '!';
int    Translator::Rules::CharsToReadIndex = 1;
int    Translator::Rules::CharsToReadOffset = CharsToReadIndex+1;
int    Translator::Rules::CommandIndex = 2;
int8_t Translator::Rules::ReadChar = 'r';
int8_t Translator::Rules::WriteChar = 'w';
int8_t Translator::Rules::OptionChar = 'o';
int8_t Translator::Rules::ErrorChar = 'e';
//int    Translator::Rules::ChannelIndex = 3;
int    Translator::Rules::DataIndex = 3;
int    Translator::Rules::DataLength = 2;

int    Translator::Rules::ReadCmdSize = CommandIndex + 1;
int    Translator::Rules::WriteCmdSize = DataIndex + DataLength;
int    Translator::Rules::ErrorCmdSize = CommandIndex + 1;


std::string Translator::MessageInfo::ToString() const {
	std::string result = "";

	// Check the command type
	switch (CommandType) {
	case NONE:
		// Incorrect command type
		return "";
	case READ:
		result += 'r';
		result += std::to_string(Channel);
		result += '(';
		result += std::to_string(DacValue);
		result += ')';
		break;
	case WRITE:
		result += 'w';
		result += std::to_string(Channel);
		result += '(' + std::to_string(DacValue) + ')';
		break;
	case OPTION:
		// Not implemented yet
		return "";
	case ERR:
		result += 'e';
		break;
	default:
		return "";
	}
	return result;
}

Translator::Translator() {}

Translator::~Translator() {}

bool Translator::Translate(MessageInfo info, int8_t** cmd, int* const cmdSize) {
	// Translation is based on the command type
	switch (info.CommandType) {
		case MessageInfo::NONE:
			// Incorrect command type value
			return false;
		case MessageInfo::READ:
			*cmdSize = Rules::ReadCmdSize;
			*cmd = new int8_t[Rules::ReadCmdSize+1]; //+1 to allow for '\0'
			(*cmd)[Rules::StartCharIndex] = Rules::StartChar;
			(*cmd)[Rules::CharsToReadIndex] = Rules::ReadCmdSize-Rules::CharsToReadOffset;
			(*cmd)[Rules::CommandIndex] = Rules::ReadChar;
			//(*cmd)[Rules::ChannelIndex] = info.Channel;
			(*cmd)[Rules::ReadCmdSize] = '\0';
			break;
		case MessageInfo::WRITE:
			*cmdSize = Rules::WriteCmdSize;
			*cmd = new int8_t[Rules::WriteCmdSize+1];
			(*cmd)[Rules::StartCharIndex] = Rules::StartChar;
			(*cmd)[Rules::CharsToReadIndex] = Rules::WriteCmdSize - Rules::CharsToReadOffset;
			(*cmd)[Rules::CommandIndex] = Rules::WriteChar;
			//(*cmd)[Rules::ChannelIndex] = info.Channel;
			for (int i = 0; i < Rules::DataLength; i++)
				(*cmd)[Rules::DataIndex + i] = (info.DacValue >> (Rules::DataLength - i - 1) * 8) & 0xFF;
			(*cmd)[Rules::WriteCmdSize] = '\0';
			break;
		case MessageInfo::OPTION:
			// Not implemented yet
			return false;
		case MessageInfo::ERR:
			*cmdSize = Rules::ErrorCmdSize;
			*cmd = new int8_t[Rules::ErrorCmdSize + 1];
			(*cmd)[Rules::StartCharIndex] = Rules::StartChar;
			(*cmd)[Rules::CharsToReadIndex] = Rules::ErrorCmdSize - Rules::CharsToReadOffset;
			(*cmd)[Rules::CommandIndex] = Rules::ErrorChar;
			(*cmd)[Rules::ErrorCmdSize] = '\0';
			break;
		default:
			return false;
	}

	return true;
}

void Translator::Translate(const int8_t* const cmd, int cmdSize, MessageInfo* const info) {
	// Check starting character
	if (cmd[Rules::StartCharIndex] != Rules::StartChar)
		throw TranslatorStartCharMissingException(cmd,cmdSize);
	// Check length of command
	if (cmd[Rules::CharsToReadIndex] + Rules::CharsToReadOffset > cmdSize)
		throw TranslatorMessageTooShortException(cmd, cmdSize);
	if (cmd[Rules::CharsToReadIndex] + Rules::CharsToReadOffset < cmdSize)
		throw TranslatorMessageTooLongException(cmd, cmdSize);

	// Populate info
	switch (cmd[Rules::CommandIndex]) {
	case 'r':
		info->CommandType = MessageInfo::READ;
		//info->Channel = cmd[Rules::ChannelIndex];
		info->DacValue = ByteUnshift(cmd + Rules::DataIndex, Rules::DataLength);
		break;
	case 'w':
		info->CommandType = MessageInfo::WRITE;
		//info->Channel = cmd[Rules::ChannelIndex];
		info->DacValue = ByteUnshift(cmd + Rules::DataIndex, Rules::DataLength);
		break;
	case 'o':
		info->CommandType = MessageInfo::OPTION;
		break;
	case 'e':
		info->CommandType = MessageInfo::ERR;
		break;
	default:
		throw TranslatorIncorrectCommandException(cmd, cmdSize);
	}
}
