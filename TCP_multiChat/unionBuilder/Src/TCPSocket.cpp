#include "masterHeader.h"

#pragma region Common
TCPSocket::~TCPSocket()
{
	closesocket(mSocket);
}

int32_t TCPSocket::Send(const void* inData, size_t inLen)
{
	int byteSentCount = send(mSocket, static_cast<const char*>(inData), inLen, 0);
	
	if(byteSentCount < 0)
	{
		SocketUtil::ReportError("TCPSocket::Send");
		return SocketUtil::GetLastError();
	}
	
	return byteSentCount;
}

int32_t TCPSocket::Receive(void* inData, size_t inLen)
{
	int byteReceiveCount = recv(mSocket, static_cast<char*>(inData), inLen, 0);
	
	if(byteReceiveCount < 0)
	{
		SocketUtil::ReportError("TCPSocket::Receive");
		return SocketUtil::GetLastError();
	}

	return byteReceiveCount;
}

#pragma endregion

#pragma region Server
int TCPSocket::Bind(const SocketAddress& inAddress)
{
	int err = bind(mSocket, &inAddress.mSockAddr, inAddress.GetSize());
	if(err != 0)
	{
		SocketUtil::ReportError("TCPSocket::Bind");
		return SocketUtil::GetLastError();
	}
	return NO_ERROR;
}

int TCPSocket::Listen(int inBackLog)
{
	int err = listen(mSocket, inBackLog);
	if(err != 0)
	{
		SocketUtil::ReportError("TCPSocket::Listen");
		return SocketUtil::GetLastError();
	}
	return NO_ERROR;
}

TCPSocketPtr TCPSocket::Accept(SocketAddress& inFromAddress)
{
	int length = inFromAddress.GetSize();
	SOCKET newSocket = accept(mSocket, &inFromAddress.mSockAddr, &length);

	if(newSocket != INVALID_SOCKET)
	{
		return TCPSocketPtr(new TCPSocket(newSocket));
	}
	else
	{
		SocketUtil::ReportError("TCPSocket::Accept");
		return nullptr;
	}
}

#pragma endregion 

#pragma region Client
int TCPSocket::Connect(const SocketAddress& inAddress)
{
	int err = connect(mSocket, &inAddress.mSockAddr, inAddress.GetSize());
	if (err != 0)
	{
		SocketUtil::ReportError("TCPSocket::Connect");
		return SocketUtil::GetLastError();
	}
	return NO_ERROR;
}
#pragma endregion