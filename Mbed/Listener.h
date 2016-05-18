#include "stdafx.h"

#pragma once
#include "SerialPort.h"
#include "Translator.h"

// Class listens to the given port when Listen is invoked
class Listener {
private:
	// The info of the last completed message
	Translator::MessageInfo _info;
	// Is set to true if _info was refilled
	bool _isNewMessageReceived;

	// <De grootte is 255+2 omdat
	//  255 = grootste getal dat in de message kan staan
	//  2 = grootte van de prefix>
	// Maximum size of the buffer
	static const int maxBufferSize = 255+2;
	// Grootte van de buffer, uitgelezen karakters van de poort worden hier opgeslagen
	int8_t _buffer[maxBufferSize];
	// Huidige index 
	int _currIndex;

public:
	// Default constructor, use SetSerialPort to link a port
	Listener();

	// Waits for a complete message indefinately on the given port
	bool Listen(const SerialPort& port);
	// Waits timeOutMs ms for a complete message on the given port
	bool Listen(const SerialPort& port, clock_t timeOutMs);

	// Get the last message as a MessageInfo
	Translator::MessageInfo GetLastMessageInfo() const;

	// <Deze functie kan gebruikt worden indien de mbed na de time-out toch nog het
	//  bericht gestuurd zou hebben. Dan kan de input buffer volledig leeg gemaakt
	//  worden en het vorige bericht opnieuw verzonden worden. Nu ik erover nadenk,
	//  Listen zou evengoed gebruikt kunnen worden om 1 enkel bericht over te slagen>
	// Discard all characters currently in the input buffer
	bool Sync(const SerialPort& port);


private:
	// <Dit zijn allemaal helper functies die gebruikt worden in de public functions>
	// Adds a single character to the buffer and returns whether a complete message
	// was received or not
	bool Add(int8_t buffer);
	// Adds multiple characters to the buffer and returns whether at least 1 complete
	// message was received or not
	bool Add(const int8_t* const buffer, DWORD charsToAdd);

	// The actual listening (only used in Listen)
	bool DoListen(const SerialPort& port);

	// Deprecated
	bool IsNewMessageReceived();
};
