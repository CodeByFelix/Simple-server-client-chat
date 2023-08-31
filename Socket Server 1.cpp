#include <WinSock2.h>
#include <iostream>
#include <array>
#include <string>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main()
{
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddr, clientAddr;


	//Initializing Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cerr << "Fail to Initialize winsock" << endl;
		return 1;
	}

	//Create a socket
	if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		cerr << "Fail to create socket" << endl;
		WSACleanup();
		return 1;
	}

	//Prepare the scokaddr in structure
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(8888);

	//Bind the socket
	if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		cerr << "Failed to bind Socket" << endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	cout << "Server Started\n";

	//listen for incoming connections
	listen(serverSocket, 3);

	//Accept an incoming connection
	int addrlen = sizeof(sockaddr_in);
	if ((clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &addrlen)) == INVALID_SOCKET)
	{
		cerr << "Fail to accept Connection\n";
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}

	cout << "Client Connected\n";
   
	//Receive and send
	char buffer[1024];
	//string buffer;
	//array <char, 1024> buffer;
	int bytesRead;

	while (true)
	{
		//memset(buffer, 0, sizeof(buffer));
		ZeroMemory(buffer, sizeof(buffer));
		bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (bytesRead == SOCKET_ERROR)
		{
			cerr << "Failed to receive data\n";
			break;
		}
		else if (bytesRead == 0)
		{
			cout << "CLient Disconnected\n";
			break;
		}
		else
			cout << "Received: " << buffer << endl;

		//Echo back the received data
		string outBuffer;
		outBuffer = buffer;
		if (send(clientSocket, outBuffer.c_str(), outBuffer.length(), 0) == SOCKET_ERROR)
		{
			cerr << "Failed to send Data!!!\n";
			break;
		}
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	return 0;
}
