#include "stdafx.h"
#include "Translator.h"

#pragma once
// Asks the user all necessary questions to fill MessageInfo
class Interactor {
private:
	// The different question states
	enum EState {
		ReadWrite,
		PortInfo,
		Channel,
		Voltage,
		Option,
		End
	};

	// The current state
	EState _state;
	// Highest DacValue value possible
	int maxDacValue;

	// Contains message info
	Translator::MessageInfo _msgInfo;
	bool _portInfo;


public:
	// Default constructor
	Interactor();

	// Ask the questions to the user
	bool AskQuestions();
	// Formats the MessageInfo as a string
	std::string ToString() const;

	// Currenctly unused
	void SetMaxDacValue(int16_t value);
	int GetMaxDacValue() const;

	bool NeedPortInfo() const;

	// Get the MessageInfo as inputted by the last quiz
	Translator::MessageInfo GetMessageInfo() const;

private:
	// <Deze functies zijn helper functies en worden gebruikt in bovenstaande
	//  functies voor de makkelijkheid.>
	// Ask about Read/Write/Option
	void ReadWriteQuestion();
	// Ask what port info to display
	void PortInfoQuestion();
	// Ask about channel (not used)
	void ChannelQuestion();
	// Ask about voltage (or DAC)
	void VoltageQuestion();

	// Reset values in _msgInfo
	void ResetValues();
	// Set read in _msgInfo
	void SetRead();
	// Set write in _msgInfo
	void SetWrite();
	// Set channel in _msgInfo (not used)
	void SetChannel(int ch);
	// Set data in _msgInfo
	void SetData(int value);
};
