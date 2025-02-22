#include "masterHeader.h"

int main()
{
	//WSAStartup
	SocketUtil::StaticInit();

	//initialize serverSocketPtr
	TCPSocketPtr listenSockPtr = SocketUtil::CreateTCPSocket(INET);

	//make address 
	SocketAddressPtr serverAddressPtr = SocketAddressFactory::CreateIPv4FromString(DEFAULT_IP_LOCAL);

	//bind server
	listenSockPtr->Bind(*serverAddressPtr);
	
	//listen sock from server
	listenSockPtr->Listen(SOMAXCONN /*default is 32*/);

	//logic variable
	vector<TCPSocketPtr> tcpSockPtrs_in;
	vector<TCPSocketPtr> tcpSockPtrs_out;
	tcpSockPtrs_in.push_back(listenSockPtr);

	bool isRunning = true;
	char bufRecv[DEFAULT_MAX_BUF_LEN];
	
	//utility variables
	const string MSG_WELCOME_TO_SERVER = "welcome to server\n";
	const string MSG_SHUTDOWN_SERVER = "Server is shutting down. Goodbye\n";
	
	while(isRunning)
	{
		if (!SocketUtil::Select(&tcpSockPtrs_in, &tcpSockPtrs_out, nullptr, nullptr, nullptr, nullptr))
			continue;

		for(const TCPSocketPtr& sockPtr : tcpSockPtrs_out)
		{
			if(sockPtr == listenSockPtr)
			{
				//add client to inVectors
				SocketAddress newClientAddress;
				auto newSockPtr = listenSockPtr->Accept(newClientAddress);
				tcpSockPtrs_in.push_back(newSockPtr);

				//send hello to client
				newSockPtr->Send(MSG_WELCOME_TO_SERVER.c_str(), MSG_WELCOME_TO_SERVER.size() + 1);
			}
			else
			{
				int byteRecv = sockPtr->Receive(bufRecv, DEFAULT_MAX_BUF_LEN);
				if (byteRecv > 0)
				{
					for (const TCPSocketPtr& sendSockPtrs : tcpSockPtrs_in)
						sendSockPtrs->Send(bufRecv, byteRecv);
				}
				else
					isRunning = false;
			}
		}
	}

	SocketUtil::CleanUp();
	system("pause");
	return 0;
}

//
//int main()
//{
//	auto listenSock = INVALID_SOCKET;
//
//	struct addrinfo *result = NULL;
//	struct addrinfo hints;
//
//	const string DEFAULT_PORT = "54000";
//	const int DEFAULT_BUFLEN = 512;
//
//	char recvbuf[DEFAULT_BUFLEN];
//	int recvbuflen = DEFAULT_BUFLEN;
//
//	fd_set mainSet, copySet;
//
//	int iResult;
//
//
//	//WSAStartup
//	SocketUtil::StaticInit();
//
//
//	// initialize winsock
//	ZeroMemory(&hints, sizeof(hints));
//	hints.ai_family = AF_INET;
//	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_protocol = IPPROTO_TCP;
//	hints.ai_flags = AI_PASSIVE;
//
//	//resolve the server address and port
//	iResult = getaddrinfo(NULL, DEFAULT_PORT.c_str(), &hints, &result);
//	if (iResult != 0)
//	{
//		std::cerr << "getaddrinfo error #" << WSAGetLastError() << std::endl;
//		WSACleanup();
//		return 1;
//	}
//
//	//create SOCKET for connecting to server
//	listenSock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
//	if (listenSock == INVALID_SOCKET)
//	{
//		std::cerr << "socket error #" << WSAGetLastError() << std::endl;
//		freeaddrinfo(result);
//		WSACleanup();
//		return 1;
//	}
//
//	//setup TCP listeningSocket
//	iResult = bind(listenSock, result->ai_addr, static_cast<int>(result->ai_addrlen));
//	if (iResult == SOCKET_ERROR)
//	{
//		std::cerr << "bind error #" << WSAGetLastError() << std::endl;
//		freeaddrinfo(result);
//		closesocket(listenSock);
//		WSACleanup();
//		return 1;
//	}
//
//	freeaddrinfo(result);
//
//	iResult = listen(listenSock, SOMAXCONN);
//	if (iResult == SOCKET_ERROR)
//	{
//		cerr << "listen error #" << WSAGetLastError() << endl;
//		closesocket(listenSock);
//		WSACleanup();
//		return 1;
//	}
//
//	FD_ZERO(&mainSet);
//	FD_SET(listenSock, &mainSet);
//
//
//	bool running = true;
//	while (running)
//	{
//		copySet = mainSet;
//		int sockCount = select(0, &copySet, nullptr, nullptr, nullptr);
//
//		for (int iSock = 0; iSock < sockCount; iSock++)
//		{
//			SOCKET sock = copySet.fd_array[iSock];
//
//			//accept new client
//			if (sock == listenSock)
//			{
//				SOCKET client = accept(listenSock, nullptr, nullptr);
//
//				if (client == INVALID_SOCKET)
//				{
//					std::cout << "accept error #" << WSAGetLastError() << endl;
//					continue;
//				}
//
//				FD_SET(client, &mainSet);
//
//				string msg = "welcome to server :" + std::to_string(listenSock) + "\r\n";
//				send(client, msg.c_str(), msg.size() + 1, 0);
//			}
//			//recv and send msg
//			else
//			{
//				ZeroMemory(recvbuf, recvbuflen);
//				int byteReceived = recv(sock, recvbuf, recvbuflen, 0);
//
//				//TODO : this code is for PUTTY error(send msg twice)
//				if ((string)recvbuf == "\r\n" || (string)recvbuf == "\n")
//					continue;
//
//				if (byteReceived <= 0)
//				{
//					if (byteReceived == 0)
//						cout << "connection closed \n";
//					else
//						cout << "receive error #" << WSAGetLastError() << endl;
//
//					closesocket(sock);
//					FD_CLR(sock, &mainSet);
//				}
//				else
//				{
//					if (recvbuf[0] == '\\')
//					{
//						string cmd = string(recvbuf, byteReceived);
//						if (cmd == "\\quit")
//						{
//							running = false;
//							break;
//						}
//
//						// Unknown command
//						continue;
//					}
//
//					for (int iSet = 0; iSet < mainSet.fd_count; iSet++)
//					{
//						SOCKET outSock = mainSet.fd_array[iSet];
//						if (outSock != listenSock && outSock != sock)
//						{
//							std::ostringstream outstream;
//							outstream << "SOCKET #" << sock << " : " << recvbuf << "\r\n";
//							send(outSock, outstream.str().c_str(), outstream.str().size() + 1, 0);
//						}
//					}
//				}
//			}
//		}
//	}
//
//	FD_CLR(listenSock, &mainSet);
//	closesocket(listenSock);
//
//	string shutDownMsg = "Server is shutting down. Goodbye\n";
//
//	while (mainSet.fd_count>0)
//	{
//		SOCKET sock = mainSet.fd_array[0];
//
//		send(sock, shutDownMsg.c_str(), shutDownMsg.size() + 1, 0);
//
//		FD_CLR(sock, &mainSet);
//		closesocket(sock);
//	}
//
//	SocketUtil::CleanUp();
//	system("pause");
//	return 0;
//}