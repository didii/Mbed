// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Interactor.h"
#include "SerialPort.h"
#include "Translator.h"

#define READ_EXISTING_TEST
#define TRANSLATOR_TEST
#define CHAR_TEST

//TODO: Create Formatter

int main() {

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
	Translator::MessageInfo info;
	info.CommandType = Translator::MessageInfo::WRITE;
	info.Channel = 1;
	info.DAC = 1;

	int8_t* rawMsg;
	std::string strMsg;
	Translator::Translate(info, &rawMsg);
	Translator::Translate(info, &strMsg);
	std::cout << rawMsg << std::endl << strMsg << std::endl;
#endif

#ifdef CHAR_TEST
	for (int i = 0; i < 256; i++)
		std::cout << i << ": " << static_cast<char>(i) << std::endl;

	std::cin.get();
#endif
	
	return 0;
}

