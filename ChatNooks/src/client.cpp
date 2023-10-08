#include "net_functions.hpp"
#include "client_stream.hpp"

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
	if(!ConnectToServerResult(ConnectSocket, result)) return 0;

	int iResult;
	int recv_buff_len = DEFAULT_BUFLEN;

	while(true)
	{
		char msg_buff[DEFAULT_BUFLEN];
		int data_len = ReadMessageBuffer(&msg_buff[0]);
		if(data_len > 0)
		{
			iResult = send(ConnectSocket, &msg_buff[0],
				data_len, 0);
		}
		else if(data_len == -1) break;
		else printf("Empty data cannot be sent. \n");
	}

	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}