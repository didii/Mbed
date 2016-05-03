#include "stdafx.h"
#include "Translator.h"


Translator::Translator() {}

Translator::~Translator() {}

bool Translator::Translate(MessageInfo info, int8_t** cmd) {
	switch (info.CommandType) {
		case MessageInfo::NONE:
			return false;
		case MessageInfo::READ:
			*cmd = new int8_t[Rules::ReadCmdSize];
			*cmd[0] = Rules::StartChar;
			*cmd[Rules::ReadWriteIndex] = Rules::WriteChar;
			*cmd[Rules::ChannelIndex] = info.Channel;
			*cmd[Rules::ReadCmdSize - 1] = Rules::EndChar;
			break;
		case MessageInfo::WRITE:
			*cmd = new int8_t[6];
			*cmd[0] = Rules::StartChar;
			*cmd[Rules::ReadWriteIndex] = Rules::WriteChar;
			*cmd[Rules::ChannelIndex] = info.Channel;
			for (int i = 0; i < Rules::DataLength; i++)
				*cmd[Rules::DataIndex + i] = (info.DAC >> (Rules::DataLength - i - 1) * 8) & 0xFF;
			*cmd[Rules::WriteCmdSize - 1] = Rules::EndChar;
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
			*msg += "(" + std::to_string(info.DAC) + ")";
			break;
		case MessageInfo::OPTION:
			// Not implemented yet
			return false;
		default:
			return false;
	}
	return true;
}
