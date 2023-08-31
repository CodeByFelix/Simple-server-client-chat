#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

#pragma comment (lib, "ws2_32.lib")
using namespace std;

int main()
{
	WSADATA wsaData;
	SOCKET clientSocket;
	SOCKADDR_IN serverAddr;

	//Initialize Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cerr << "Failed to Initize Winsock" << endl;
		return 1;
	}

	//Create a Socket
	if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		cerr << "Failed to creak socket" << endl;
		return 1;
	}

	//Prepare the socket in structure
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8888);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//connect to server
	if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
	{
		cerr << "Failed to connect to server" << endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}

	cout << "Connected to the server\n";

	//Sending and receiving data
	char buffer[1024];
	string sendBuffer;
	int bytesRead;

	while (true)
	{
		//send data to server
		cout << "Enter a message: ";
		getline(cin, sendBuffer);
		//cin.getline(sendBuffer, sizeof(sendBuffer));

		if (send(clientSocket, sendBuffer.c_str(), sendBuffer.length()+1, 0) == SOCKET_ERROR)
		{
			cerr << "Failed to send Data" << endl;
			break;
		}
		cout << "Data sent to server.\n";

		//Receive Data
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
			cerr << "Server Disconnected\n";
			break;
		}
		else
			cout << "Received: " << buffer << endl;
	}

	closesocket(clientSocket);
	WSACleanup();

	return 0;
}