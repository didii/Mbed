// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Interactor.h"
#include "SerialPort.h"
#include "Listener.h"
#include "Translator.h"

// Define to run the actual program
#define PROGRAM
// A random test to test stuff
//#define RANDOM_TEST
// Testing the ByteShift and ByteUnshift functions
//#define BYTESHIFT_TEST
// Testing the SerialPort::ReadExisting functions
//#define READ_EXISTING_TEST
// Testing the Translator class
//#define TRANSLATOR_TEST


bool Retry(bool* const retry);

int main() {

#ifdef PROGRAM
	std::wstring portName = L"COM3";
	// Create connection to the COM _port
	SerialPort port(portName);
	if (!port.IsOpen()) {
		std::cout << "Error: could not connect to " << portName.c_str() << std::endl;
		return 0;
	}
	// Create the question master
	Interactor interactor;
	// Create a listener
	Listener listener;

	std::cout << "Connection established\n";
	{
		std::string buffer;
		port.ReadExisting(&buffer);
		std::cout << buffer << std::endl;
		
	}
	bool running = true;
	while (running) {
		// Start asking questions
		if (!interactor.AskQuestions())
			break;

		if (interactor.NeedPortInfo()) {
			std::cout << "Waiting characters: " << port.CharactersWaiting() << std::endl;
			if (port.CharactersWaiting() > 0) {
				std::cout << "Characters: ";
				std::string buffer;
				port.ReadExisting(&buffer);
				std::cout << buffer << "\n";
			}
			std::cout << std::endl;
			continue;
		}

		std::cout << "Sending: " << interactor.ToString() << std::endl;

		// Translate the CommandInfo to raw
		int8_t* rawMsg;
		int rawMsgSize;
		Translator::Translate(interactor.GetMessageInfo(), &rawMsg, &rawMsgSize);

		bool retry = false;
		do { // Send the message
			if (port.Write(rawMsg, rawMsgSize) != 0) {
				std::cout << "An error occured while sending. ";
				if (!Retry(&retry))
					running = false;
				continue;
			}
			//Sleep(200);
			//int8_t buffer[258];
			//DWORD size;
			////std::string buffer;
			//port.ReadExisting(buffer, &size);
			//for (int i = 0; i < size; i++)
			//	std::cout << "'" << buffer[i] << "' (" << (int)buffer[i] << ")" << "\n";
			//std::cout << "\n";
			//break;


			std::cout << "Message sent.\nWaiting for answer...\n";

			// Wait for answer
			if (listener.Listen(port, 1)) {
				// An answer was received
				auto msg = listener.GetLastMessageInfo();
				std::cout << "Answer: " << listener.GetLastMessageInfo().ToString() << std::endl;
			} else {
				bool correctInput;
				std::cout << "Got no message back. ";
				if (!Retry(&retry))
					running = false;
				continue;
			}
		} while (retry);
		if (port.CharactersWaiting() > 0) {
			std::cout << "Reading sent messages:\n";
			std::string buffer;
			port.ReadExisting(&buffer);
			std::cout << buffer << std::endl;
		}
		std::cout << std::endl;
	}

	std::cout << "Terminating program...\n";
	return 0;
#endif

#ifdef RANDOM_TEST
#endif

#ifdef BYTESHIFT_TEST
	int num = 256;
	int numFrames = 2;
	int8_t* frames;
	ByteShift(num, numFrames, &frames);
	std::cout << "Num: " << num << std::endl;
	std::cout << "Shifted: ";
	for (int i = 0; i < numFrames; i++)
		std::cout << (int)frames[i] << " ";
	std::cout << std::endl;
	std::cout << "Unshifted: " << ByteUnshift(frames, numFrames) << std::endl;
#endif


#ifdef READ_EXISTING_TEST
	// Try to open the _port
	std::cout << "Opening _port...\n";
	SerialPort comm;
	if (comm.OpenPort(L"COM3")) {
		std::cerr << "Could not open _port, exiting...";
		return 0;
	}

	// Read all existing characters
	std::cout << "Port opened, reading buffer...\n";
	std::string buffer = "";
	comm.ReadExisting(&buffer);
	if (buffer.length() > 0)
		std::cout << std::string(buffer);
	else
		std::cout << "Empty buffer\n";


	for (int i = 0; i < 10; i++) {
		comm.Write("|");
		Sleep(50);
		comm.ReadExisting(&buffer);
		if (buffer.length() > 0)
			std::cout << std::string(buffer);
		Sleep(450);
	}
	comm.Close();
#endif

#ifdef TRANSLATOR_TEST
	// Create new message
	Translator::MessageInfo info;
	info.CommandType = Translator::MessageInfo::WRITE;
	info.Channel = 1;
	info.DacValue = 1;

	// Translate to raw message
	int rawMsgSize;
	int8_t* rawMsg;
	Translator::Translate(info, &rawMsg, &rawMsgSize);
	std::cout << rawMsg << std::endl;

	// Translate to string
	std::cout << info.ToString() << std::endl;

	// Translate back to MessageInfo
	Translator::MessageInfo info2;
	Translator::Translate(rawMsg, rawMsgSize, &info2);
	switch (info2.CommandType) {
		case Translator::MessageInfo::NONE:
			std::cout << "None";
			break;
		case Translator::MessageInfo::READ:
			std::cout << "Read";
			break;
		case Translator::MessageInfo::WRITE:
			std::cout << "Write";
			break;
		case Translator::MessageInfo::OPTION:
			std::cout << "Option";
			break;
		case Translator::MessageInfo::ERR:
			std::cout << "Error";
			break;
		default:
			throw;
	}
	std::cout << ": " << info2.Channel << "(" << info2.DacValue << ")\n";
#endif
	
	return 0;
}

bool Retry(bool* const retry) {
	while (true) {
		// No answer in the timeout
		std::cout << "Retry? (y/n) ";
		char in;
		std::cin >> in;
		if (std::cin.fail()) {
			std::cout << "Incorrect input...\n";
			continue;
		}
		switch (in) {
		case 'y':
			*retry = true;
			return true;
		case 'n':
			*retry = false;
			return true;
		case 'q':
			*retry = false;
			return false;
		default:
			std::cout << "Incorrect input...\n";
			break;
		}
	}

}