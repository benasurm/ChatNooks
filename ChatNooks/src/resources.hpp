#pragma once

#include <stdio.h>
#include <string>

const std::string LogoFilePath = "../input/logo.txt";

struct Messages
{
	void StartupMessage()
	{
		printf("To create a chat room, press key '1' on your keyboard. \n");
		printf("To join a chat room, press key '2' on your keyboard. \n");
	}

	void CreatingChatRoom(int option)
	{
		if(option == 1)
			printf("Creating chat room....\n");
		else if(option == 2)
			printf("Joining chat room....\n");
	}

	void UnspecifiedIPAddr(char *argument)
	{
		printf("IP address not specified. Usage:\n%s <IP>\n", argument);
	}

	void WSAStartupFailed(int result)
	{
		printf("WSAStartup failed: %d\n", result);
	}

	void WSAStartupSuccess()
	{
		printf("WSAStartup has been started successfully\n");
	}

	void getaddrinfoFailed(int result)
	{
		printf("getaddrinfo failed %d\n", result);
	}

	void getaddrinfoComplete()
	{
		printf("getaddrinfo has been completed successfully. \n");
	}

	void UnableToConnectToServer()
	{
		printf("Unable to connect to server. \n");
	}

	void BindingFailed(int error_code)
	{
		printf("Bind failed with error: %d\n", error_code);
	}

	void ListenFailed(int error_code)
	{
		printf("Listen failed with error: %ld\n", error_code);
	}

	void ConnectionToServerFailed(int error_code)
	{
		printf("Connection to server failed: %ld\n", error_code);
	}

	void ConnectionToServerSuccessfull()
	{
		printf("Connection to server successfull. \n");
	}

	void ConnectionRefused()
	{
		printf("Reason: Connection refused by target host.");
	}
};

enum InputType { start_opt };
enum RoomOption { server, client };
