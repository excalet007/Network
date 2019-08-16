#include "masterHeader.h"

bool isRunning;

DWORD WINAPI inputThread(void *data)
{
	TCPSocketPtr clientPtr = *reinterpret_cast<TCPSocketPtr*>(data);

	string userInput;
	while (true)
	{
		getline(cin, userInput);

		if (!userInput.empty())
			clientPtr->Send(userInput.c_str(), userInput.size() + 1);

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
	//startup winsock
	SocketUtil::StaticInit();

	//make socketPtr
	TCPSocketPtr clientPtr = SocketUtil::CreateTCPSocket(INET);

	//make address from string
	SocketAddressPtr serverAddressPtr = SocketAddressFactory::CreateIPv4FromString(DEFAULT_IP_LOCAL);

	//connect
	clientPtr->Connect(*serverAddressPtr);

	//loop escape
	isRunning = true;

	//send thread
	HANDLE inputThreadHandler;
	inputThreadHandler = CreateThread(NULL, 0, inputThread, reinterpret_cast<void*>(&clientPtr), 0, NULL);

	//receive
	char bufRecv[DEFAULT_MAX_BUF_LEN];
	int byteRecv;

	while (isRunning)
	{
		ZeroMemory(bufRecv, DEFAULT_MAX_BUF_LEN);
		byteRecv = clientPtr->Receive(bufRecv, DEFAULT_MAX_BUF_LEN);
		if (byteRecv > 0)
			cout << "RECEIVED > " << string(bufRecv, 0, byteRecv) << endl;
	}

	//shut down system
	CloseHandle(inputThreadHandler);
	SocketUtil::CleanUp();

	system("pause");
	return 0;
}

//DWORD WINAPI inputThread(void *data)
//{
//	SOCKET client = *reinterpret_cast<SOCKET*>(data);
//
//	string userInput;
//	while (true)
//	{
//		getline(cin, userInput);
//		if (!userInput.empty())
//			send(client, userInput.c_str(), userInput.size() + 1, 0);
//		else
//		{
//			isRunning = false;
//			break;
//		}
//	}
//	return 1;
//}

//
//int main()
//{
//	SOCKET client = INVALID_SOCKET;
//	sockaddr_in hint;
//
//	HANDLE inputThreadHandler;
//
//	char buf[DEFAULT_MAX_BUF_LEN];
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
//		ZeroMemory(buf, DEFAULT_MAX_BUF_LEN);
//		byteRecv = recv(client, buf, DEFAULT_MAX_BUF_LEN, 0);
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