#include "stdafx.h"
#include "Interactor.h"


Interactor::Interactor(): maxDacValue(4096) {
	ResetValues();
}

bool Interactor::AskCommando() {
	ResetValues();
	_state = ReadWrite;
	while (_state != End) {
		switch (_state) {
			case ReadWrite:
				ReadWriteQuestion();
				break;
			case Channel:
				ChannelQuestion();
				break;
			case Voltage:
				VoltageQuestion();
				break;
			default:
				break;
		}
	}
	if (_msgInfo.CommandType == Translator::MessageInfo::NONE)
		return false;
	return true;
}

std::string Interactor::ToString() const {
	std::string result;
	Translator::Translate(_msgInfo, &result);
	return result;
}

void Interactor::SetMaxDacValue(int16_t value) {
	maxDacValue = value;
}

int Interactor::GetMaxDacValue() const {
	return maxDacValue;
}

Translator::MessageInfo Interactor::GetCommandInfo() const {
	return _msgInfo;
}

void Interactor::ReadWriteQuestion() {
	// Ask the user whether they want to read or write data
	std::cout << "Read or write (r/w)? ";
	char in_char;
	std::cin >> in_char;

	// Check inputted chracter
	switch (in_char) {
	case 'r': // read commando
		SetRead();
		_state = Channel;
		break;
	case 'w':
		SetWrite();
		_state = Channel;
		break;
	case 'q':
		std::cout << "Quitting...\n";
		_state = End;
	default:
		std::cout << "Incorrect input\n";
		break;
	}
}

void Interactor::ChannelQuestion() {
	// Ask the user from which channel to read/write
	switch (_msgInfo.CommandType) {
		case Translator::MessageInfo::NONE:
			std::cout << "ReadWrite does not have a value, returning to state ReadWrite\n";
			_state = ReadWrite;
			return;
		case Translator::MessageInfo::READ:
			std::cout << "Read from ";
			break;
		case Translator::MessageInfo::WRITE:
			std::cout << "Write to ";
			break;
		case Translator::MessageInfo::OPTION:
			_state = Option;
			return;
		default:
			std::cout << "Something went horribly wrong...\n";
			_state = End;
			return;
	}
	std::cout << " which channel (1-4)? ";
	int value;
	std::cin >> value;
	// Check if std::cin failed
	if (std::cin.fail()) {
		std::cout << "Incorrect input\n";
		std::cin.clear(); // Important! std::cin does not read again until all errors are cleared
		return;
	}

	// If succesful, check value
	if (value >= 1 && value <= 4) {
		SetChannel(value);
		if (_msgInfo.CommandType == Translator::MessageInfo::READ) {
			SetData(0);
			_state = End;
		} else if (_msgInfo.CommandType == Translator::MessageInfo::WRITE)
			_state = Voltage;
	} else
		std::cout << "Incorrect input\n";
}

void Interactor::VoltageQuestion() {
	//TODO: Change DAC value to Voltage
	// Ask DAC value
	std::cout << "Set DAC value to? ";
	int value;
	std::cin >> value;
	// Check if std::cin failed
	if (std::cin.fail()) {
		std::cout << "Incorrect input\n";
		std::cin.clear(); // Important! std::cin does not read again until all errors are cleared
		return;
	}

	// If succesful, set it and end the questions
	SetData(value);
	_state = End;
}

void Interactor::ResetValues() {
	// Reset all values to their original ones
	_state = ReadWrite;
	_msgInfo.CommandType = Translator::MessageInfo::NONE;
	_msgInfo.Channel = -1;
}

void Interactor::SetRead() {
	_msgInfo.CommandType = Translator::MessageInfo::READ;
}

void Interactor::SetWrite() {
	_msgInfo.CommandType = Translator::MessageInfo::WRITE;
}

void Interactor::SetChannel(int ch) {
	_msgInfo.Channel = ch;
}

void Interactor::SetData(int data) {
	_msgInfo.DAC = data;
}