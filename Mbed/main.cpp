// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Interactor.h"
#include "SerialPort.h"
#include "Translator.h"

//#define RANDOM_TEST
//#define BYTESHIFT_TEST
#define READ_EXISTING_TEST
#define TRANSLATOR_TEST
//#define CHAR_TEST

//TODO: Create Formatter

int main() {

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
	// Try to open the port
	std::cout << "Opening port...\n";
	SerialPort comm;
	if (comm.OpenPort(L"COM3")) {
		std::cerr << "Could not open port, exiting...";
		return 0;
	}

	// Read all existing characters
	std::cout << "Port opened, reading buffer...\n";
	std::string buffer = "";
	comm.ReadExisiting(buffer);
	if (buffer.length() > 0)
		std::cout << std::string(buffer);
	else
		std::cout << "Empty buffer\n";


	for (int i = 0; i < 10; i++) {
		comm.Write("|");
		Sleep(50);
		comm.ReadExisiting(buffer);
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
	std::string strMsg;
	Translator::Translate(info, &strMsg);
	std::cout << strMsg << std::endl;

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
		default:
			throw;
	}
	std::cout << ": " << info2.Channel << "(" << info2.DacValue << ")\n";
#endif

#ifdef CHAR_TEST
	for (int i = 0; i < 256; i++)
		std::cout << i << ": " << static_cast<char>(i) << std::endl;

	std::cin.get();
#endif
	
	return 0;
}

