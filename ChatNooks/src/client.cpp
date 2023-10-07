#include "net_functions.hpp"

void GetIPAddress(int argc, char *argv[],
	std::string &IP_ADDRESS, Messages &mess_obj)
{
	if(argc < 2)
	{
		mess_obj.UnspecifiedIPAddr(argv[0]);
		return;
	}
	IP_ADDRESS = argv[1];
}

bool ConnectToServerResult(SOCKET &ConnectSocket, addrinfo *ptr)
{
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
		ptr->ai_protocol);

	int iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if(iResult == SOCKET_ERROR)
	{
		mess_obj.ConnectionToServerFailed(WSAGetLastError());
		if(WSAGetLastError() == 10061)
			mess_obj.ConnectionRefused();
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
		return false;
	}
	freeaddrinfo(ptr);
	if(ConnectSocket == INVALID_SOCKET)
	{
		mess_obj.UnableToConnectToServer();
		WSACleanup();
		return false;
	}
	mess_obj.ConnectionToServerSuccessfull();
	return true;
}

int main(int argc, char *argv[])
{
	std::string IP_ADDRESS;
	GetIPAddress(argc, argv, IP_ADDRESS, mess_obj);
	
	if(IP_ADDRESS.empty()) return 0;

	WSADATA wsaData;

	// Initialize Winsock
	if(!WSAStartupResult(wsaData)) return 0;

	struct addrinfo *result = NULL, *ptr = NULL, hints;
	SetupAddrInfo(hints);

	// Resolve the server address and port
	if(!GetAddressInfoResult(&IP_ADDRESS[0], &hints, &result)) return 0;

	SOCKET ConnectSocket = INVALID_SOCKET;
	ConnectToServerResult(ConnectSocket, result);

	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}