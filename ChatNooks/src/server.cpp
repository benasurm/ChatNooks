#include "net_functions.hpp"

bool OpenListenSocketResult(SOCKET &ListenSocket, addrinfo *result)
{
	ListenSocket = socket(result->ai_family, 
		result->ai_socktype, result->ai_protocol);

	if(ListenSocket == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}
	printf("Listen socket opened successfully. \n");
	return true;
}

bool BindTCPPortResult(SOCKET &ListenSocket, addrinfo *ptr)
{
	int iResult = bind(ListenSocket, ptr->ai_addr, 
		(int)ptr->ai_addrlen);
	if(iResult == SOCKET_ERROR)
	{
		mess_obj.BindingFailed(WSAGetLastError());
		freeaddrinfo(ptr);
		closesocket(ListenSocket);
		WSACleanup();
		return false;
	}
	return true;
}

bool SetToListenModeResult(SOCKET &ListenSocket)
{
	if(listen(ListenSocket, MAX_CONN_COUNT) == SOCKET_ERROR)
	{
		mess_obj.ListenFailed(WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return false;
	}
	return true;
}

int main()
{
	WSADATA wsaData;
	
	// Initialize Winsock
	if(!WSAStartupResult(wsaData)) return 0;

	struct addrinfo *result = NULL, *ptr = NULL, hints;
	
	// Setup connnection options
	SetupAddrInfo(hints);

	// Get address information
	if(!GetAddressInfoResult(NULL, &hints, &result)) return 0;

	if(result == NULL);

	// Open listening socket
	SOCKET ListenSocket = INVALID_SOCKET;
	if(!OpenListenSocketResult(ListenSocket, result)) return 0;

	// Bind TCP listening port
	if(!BindTCPPortResult(ListenSocket, result)) return 0;

	// Turn on listening mode on the socket.
	if(!SetToListenModeResult(ListenSocket)) return 0;

	SOCKET ClientSocket = INVALID_SOCKET;
	printf("Listening for connections \n");

	// To do:
	// Display incoming messages from client
	// Connect multiple clients at the same time
	// Display message of one client to every client that is connected to server currently

	struct sockaddr_in client_info;
	int client_info_size = sizeof(client_info);
	ClientSocket = accept(ListenSocket, (struct sockaddr*)&client_info, &client_info_size);
	if(ClientSocket == INVALID_SOCKET)
	{
		printf("Connection accept failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 0;
	}
	char *ip = inet_ntoa(client_info.sin_addr);
	printf("Connection with client (%s) established \n", ip);

	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recv_buff_len = DEFAULT_BUFLEN;
	do
	{
		iResult = recv(ClientSocket, &recvbuf[0], recv_buff_len,
			0);
		if(iResult > 0)
		{
			printf("Bytes recevied from %s : %d\n", ip, iResult);
			printf("Message from %s: \n", ip);
			for(int i = 0; i < iResult; i++)
			{
				printf("%c", recvbuf[i]);
				if(i % 15 == 0) printf("\n");
			}
		}
		else if(iResult == 0)
		{
			printf("Connection with %s is closing...\n", ip);
			closesocket(ClientSocket);
		}
		else
		{
			printf("Recv failed: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 0;
		}
	} 
	while(iResult > 0);

	return 1;
}