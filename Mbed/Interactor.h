#include "stdafx.h"
#include "Translator.h"

#pragma once
// Asks the user all necessary questions to fill MessageInfo
class Interactor {
private:
	// The different question states
	enum EState {
		ReadWrite,
		Channel,
		Voltage,
		Option,
		End
	};

	// The current state
	EState _state;
	// Highest DAC value possible
	int maxDacValue;

	// Contains message info
	Translator::MessageInfo _msgInfo;

public:
	Interactor();
	~Interactor();

	bool AskCommando();
	std::string ToString() const;

	void SetMaxDacValue(int16_t value);
	int GetMaxDacValue() const;
	Translator::MessageInfo GetCommandInfo() const;

private:
	// Helper functions
	void ReadWriteQuestion();
	void ChannelQuestion();
	void VoltageQuestion();

	void ResetValues();
	void SetRead();
	void SetWrite();
	void SetChannel(int ch);
	void SetData(int value);
};
