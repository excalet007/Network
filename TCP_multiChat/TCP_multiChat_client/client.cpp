#include "masterHeader.h"

const int PORT = 54000;
const int MAX_BUFFER_LENGTH = 1024;
const string IP = "127.0.0.1";

bool isRunning;

DWORD WINAPI inputThread(void *data)
{
	SOCKET client = *reinterpret_cast<SOCKET*>(data);

	string userInput;
	while (true)
	{
		getline(cin, userInput);
		if (!userInput.empty())
			send(client, userInput.c_str(), userInput.size() + 1, 0);
		else
		{
			isRunning = false;
			break;
		}
	}
	return 1;
}

int main()
{
	SOCKET client = INVALID_SOCKET;
	sockaddr_in hint;

	HANDLE inputThreadHandler;

	char buf[MAX_BUFFER_LENGTH];
	int byteRecv;

	isRunning = true;
	int iResult;

	SocketUtil::StaticInit();

	client = socket(AF_INET, SOCK_STREAM, 0);
	if (client == INVALID_SOCKET)
	{
		cerr << "socket error #" << WSAGetLastError() << endl;
		WSACleanup();
		return 1;
	}

	hint.sin_family = AF_INET;
	hint.sin_port = htons(PORT);
	inet_pton(AF_INET, IP.c_str(), &hint.sin_addr);

	iResult = connect(client, (sockaddr*)&hint, sizeof(hint));
	if (iResult == SOCKET_ERROR)
	{
		cerr << "connect error #" << WSAGetLastError() << endl;
		closesocket(client);
		WSACleanup();
		return 1;
	}



	inputThreadHandler = CreateThread(NULL, 0, inputThread, reinterpret_cast<void*>(&client), 0, NULL);

	while (isRunning)
	{
		ZeroMemory(buf, MAX_BUFFER_LENGTH);
		byteRecv = recv(client, buf, MAX_BUFFER_LENGTH, 0);
		if (byteRecv > 0)
			cout << "RECEIVED > " << string(buf, 0, byteRecv) << endl;
	}

	CloseHandle(inputThreadHandler);

	//clean up
	closesocket(client);
	SocketUtil::CleanUp();
	system("pause");
	return 0;
}

//
//int main()
//{
//	SOCKET client = INVALID_SOCKET;
//	sockaddr_in hint;
//
//	HANDLE inputThreadHandler;
//
//	char buf[MAX_BUFFER_LENGTH];
//	int byteRecv;
//
//	isRunning = true;
//	int iResult;
//
//	SocketUtil::StaticInit();
//
//	client = socket(AF_INET, SOCK_STREAM, 0);
//	if (client == INVALID_SOCKET)
//	{
//		cerr << "socket error #" << WSAGetLastError() << endl;
//		WSACleanup();
//		return 1;
//	}
//
//	hint.sin_family = AF_INET;
//	hint.sin_port = htons(PORT);
//	inet_pton(AF_INET, IP.c_str(), &hint.sin_addr);
//
//	iResult = connect(client, (sockaddr*)&hint, sizeof(hint));
//	if (iResult == SOCKET_ERROR)
//	{
//		cerr << "connect error #" << WSAGetLastError() << endl;
//		closesocket(client);
//		WSACleanup();
//		return 1;
//	}
//
//
//
//	inputThreadHandler = CreateThread(NULL, 0, inputThread, reinterpret_cast<void*>(&client), 0, NULL);
//
//	while (isRunning)
//	{
//		ZeroMemory(buf, MAX_BUFFER_LENGTH);
//		byteRecv = recv(client, buf, MAX_BUFFER_LENGTH, 0);
//		if (byteRecv > 0)
//			cout << "RECEIVED > " << string(buf, 0, byteRecv) << endl;
//	}
//
//	CloseHandle(inputThreadHandler);
//
//	//clean up
//	closesocket(client);
//	SocketUtil::CleanUp();
//	system("pause");
//	return 0;
//}