#pragma once

#include <windows.h>
#include <string>
#include "net_functions.hpp"

#define ENTER_KEY_VAL 10

InputType GetInputType(KEY_EVENT_RECORD ker, char &symbol)
{
	InputType temp = itNull;
	if(ker.bKeyDown)
	{
		if(ker.uChar.AsciiChar == '~')
			temp = itCommand;
		if(ker.wVirtualKeyCode == VK_RETURN)
			temp = itEnterKey;
		else
		{
			temp = itSym;
			symbol = ker.uChar.AsciiChar;
		} 	
	}
	return temp;
}

int ReadClientInput(HANDLE &ConsoleInputHandle,
	INPUT_RECORD InputBuffer[], char *buff_begin)
{
	DWORD recordCount;
	int msg_size = 0;
	InputType input_type;
	while(true)
	{
		if(!ReadConsoleInput(ConsoleInputHandle, InputBuffer, DEFAULT_BUFLEN, 
			&recordCount))
		{
			printf("Error while reading console input \n");
			return -1;
		}
		bool command_mode = false;
		std::string command = "";
		char symbol;
		for(int i = 0; i < recordCount; i++)
		{
			if(InputBuffer[i].EventType == KEY_EVENT &&
				InputBuffer[i].Event.KeyEvent.bKeyDown)
			{
				input_type = GetInputType(InputBuffer[i].Event.KeyEvent, 
					symbol);
				if(input_type == itCommand)
					command_mode = true;
				if(input_type == itSym)
				{
					if(command_mode)
						command += symbol;
					else
					{
						printf("%c", symbol);
						*buff_begin = symbol;
						buff_begin++;
						msg_size++;
					}
				}
				if(input_type == itEnterKey)
				{
					printf("\n");
					return msg_size;
				}
			}
		}
		if(command == "quit") return -1;
	}
}

int ReadMessageBuffer(char *buff_begin)
{
	HANDLE ConsoleInputHandle;
	ConsoleInputHandle = GetStdHandle(STD_INPUT_HANDLE);

	INPUT_RECORD InputBuffer[DEFAULT_BUFLEN];
	int buffer_len = ReadClientInput(ConsoleInputHandle, InputBuffer, buff_begin);

	return buffer_len;
}
