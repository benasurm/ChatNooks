#include <fstream>
#include <iostream>
#include <windows.h>
#include "resources.hpp"
#include "key_events.hpp"

#define MAX_INPUT_LEN 256
#define PROGRAM_TEXT_COLOR 10
#define DEFAULT_TEXT_COLOR 15

using std::cout;
using std::endl;

void PrepareStartupScreen(HANDLE &ConsoleOutputHandle,
	Messages &mess_obj);

void ClearConsole();

void PrintLogo();

void ReadUserInput(HANDLE &ConsoleInputHandle, HANDLE &ConsoleOutputHandle,
	INPUT_RECORD InputBuffer[], Messages &mess_obj, InputType type);

bool KeyEventProcess(KEY_EVENT_RECORD ker, Messages &mess_obj, 
	InputType type);

int main()
{
	DWORD fdwSaveOldMode = 0;
	
	Messages mess_obj;

	InputType type = start_opt;
	RoomOption room_option;

	HANDLE ConsoleOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(ConsoleOutputHandle, &fdwSaveOldMode);
	const DWORD originalMode = fdwSaveOldMode;

	PrepareStartupScreen(ConsoleOutputHandle, mess_obj);

	HANDLE ConsoleInputHandle;
	ConsoleInputHandle = GetStdHandle(STD_INPUT_HANDLE);

	INPUT_RECORD InputBuffer[MAX_INPUT_LEN];
	ReadUserInput(ConsoleInputHandle, ConsoleOutputHandle, InputBuffer, mess_obj, type,
		room_option);

	if(room_option == server)
	{
		//StartServer();
	}
	else if(room_option == client)
	{
		// Implement call to client side
	}



	SetConsoleMode(ConsoleOutputHandle, originalMode);
	SetConsoleTextAttribute(ConsoleOutputHandle, DEFAULT_TEXT_COLOR);
}

void PrepareStartupScreen(HANDLE &ConsoleOutputHandle,
	Messages &mess_obj)
{
	SetConsoleTextAttribute(ConsoleOutputHandle, PROGRAM_TEXT_COLOR);
	ClearConsole();
	PrintLogo();
	cout << endl;
	mess_obj.StartupMessage();
}

void ClearConsole()
{
	cout << "\x1B[2J\x1B[H";
}

void PrintLogo()
{
	std::string line;
	std::ifstream inFile(LogoFilePath);
	while(getline(inFile, line))
		cout << line << endl;
}

void ReadUserInput(HANDLE &ConsoleInputHandle, HANDLE &ConsoleOutputHandle,
	INPUT_RECORD InputBuffer[], Messages &mess_obj, InputType input,
	RoomOption &room_option)
{
	DWORD recordCount;
	bool decisionMade = false;
	while(true)
	{
		if(decisionMade == true) return;
		if(!ReadConsoleInput(ConsoleInputHandle, InputBuffer, MAX_INPUT_LEN, &recordCount))
		{
			cout << "** Error in ReadConsoleInput()" << endl;
			return;
		}
		for(int i = 0; i < recordCount; i++)
		{
			if(InputBuffer[i].EventType == KEY_EVENT)
				decisionMade = KeyEventProcess(InputBuffer[i].Event.KeyEvent, mess_obj, input, 
					option);
		}
	}
}

bool KeyEventProcess(KEY_EVENT_RECORD ker, Messages &mess_obj, InputType input, RoomOption &option)
{
	KeyEvents KeyEventObj;
	if(ker.bKeyDown)
	{
		if(input == start_opt)
		{
			return KeyEventObj.SelectRoomOpearation(mess_obj, ker, option);
		}
	}
	return false;
}