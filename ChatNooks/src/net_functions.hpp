#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include "resources.hpp"

#define DEFAULT_PORT "55555"
#define DEFAULT_BUFLEN 512
#define MAX_CONN_COUNT 4

Messages mess_obj;

void SetupAddrInfo(addrinfo &hints)
{
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
}

bool WSAStartupResult(WSADATA &wsaData)
{
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) 
	{
	    mess_obj.WSAStartupFailed(iResult);
	    return false;
	}
	mess_obj.WSAStartupSuccess();
	return true;
}

bool GetAddressInfoResult(char *argument, addrinfo *hints,
	addrinfo **result)
{
	int iResult = getaddrinfo(argument, DEFAULT_PORT, hints, result);
	if(iResult != 0)
	{
		mess_obj.getaddrinfoFailed(iResult);
		WSACleanup();
		return false;
	}
	mess_obj.getaddrinfoComplete();
	return true;
}