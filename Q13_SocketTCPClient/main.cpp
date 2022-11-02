#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "WS2_32.lib") //DLL

using namespace std;

int main()
{
	WSAData wsaData;
	int Result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (Result < 0)
	{
		cout << "Winsock init Error.  " << GetLastError() << endl;
		exit(-1);
	}

	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ServerSocket == INVALID_SOCKET)
	{
		cout << "make socket error.  " << GetLastError() << endl;
		exit(-1);
	}

	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(SOCKADDR_IN));
	ServerSockAddr.sin_family = PF_INET;
	ServerSockAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //host to network long
	ServerSockAddr.sin_port = htons(12345); // host to network short

	int Status = bind(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));
	if (Status == SOCKET_ERROR)
	{
		cout << "bind error.  " << GetLastError() << endl;
		exit(-1);
	}

	Status = listen(ServerSocket, 0);
	if (Status == SOCKET_ERROR)
	{
		cout << "listen error.  " << GetLastError() << endl;
		exit(-1);
	}

	SOCKADDR_IN ClientAddrIn;
	memset(&ClientAddrIn, 0, sizeof(SOCKADDR_IN));
	int ClientLength = sizeof(ClientAddrIn);

	//blocking socket
	SOCKET ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddrIn, &ClientLength);

	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "accept error.  " << GetLastError() << endl;
		exit(-1);
	}

	const char Message[] = "Hello World";
	int SendBytes = send(ClientSocket, Message, strlen(Message) + 1, 0);
	if (SendBytes <= 0)
	{
		cout << "send error.  " << GetLastError() << endl;
		exit(-1);
	}

	char Buffer[1024] = { 0, };

	int RecvBytes = recv(ClientSocket, Buffer, 1024, 0);
	if (RecvBytes <= 0)
	{
		cout << "recv error.  " << GetLastError() << endl;
		exit(-1);
	}

	closesocket(ClientSocket);

	closesocket(ServerSocket);


	WSACleanup();
	return 0;
}