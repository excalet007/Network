#include "masterHeader.h"

bool SocketUtil::StaticInit()
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
	{
		ReportError("StartUp");
		return false;
	}
	return true;
}

void SocketUtil::CleanUp()
{
	WSACleanup();
}

void SocketUtil::ReportError(const char* inOperationDesc)
{
	cout << "error #" << GetLastError() << " _ " << inOperationDesc << endl;
}

int SocketUtil::GetLastError()
{
	return WSAGetLastError();
}

TCPSocketPtr SocketUtil::CreateTCPSocket(SocketAddressFamily inEnumFamily)
{
	SOCKET sock = socket(inEnumFamily, SOCK_STREAM, IPPROTO_TCP);

	if(sock != INVALID_SOCKET)
	{
		return TCPSocketPtr(new TCPSocket(sock));
	}
	else
	{
		ReportError("SocketUtil::CreateTCPSocket");
		return nullptr;
	}
}

fd_set* SocketUtil::FillSetFromVector(fd_set& outSet, const vector<TCPSocketPtr>* inSockPtrs, int& ioNaxNfds)
{
	if(inSockPtrs)
	{
		FD_ZERO(&outSet);
	
		for(const TCPSocketPtr& sockPtr : *inSockPtrs)
			FD_SET(sockPtr->mSocket, &outSet);

		return &outSet;
	}
	else
	{
		return nullptr;
	}
}

void SocketUtil::FillVectorFromSet(vector<TCPSocketPtr>* outSockPtrs, const vector<TCPSocketPtr>* inSockPtrs, const fd_set& inSet)
{
	if(inSockPtrs && outSockPtrs)
	{
		outSockPtrs->clear();

		for (const TCPSocketPtr& sockPtr : *inSockPtrs)
			if (FD_ISSET(sockPtr->mSocket, &inSet))
				outSockPtrs->push_back(sockPtr);
	}
}

int SocketUtil::Select	(	const	vector<TCPSocketPtr>* inReadSet,
													vector<TCPSocketPtr>* outReadSet,
										const	vector<TCPSocketPtr>* inWriteSet,
													vector<TCPSocketPtr>* outWriteSet,
										const	vector<TCPSocketPtr>* inExceptSet,
													vector<TCPSocketPtr>* outExceptSet)
{
	fd_set read, write, except;

	int nfds = 0;

	fd_set *readPtr		= FillSetFromVector(	read,		inReadSet,		nfds);
	fd_set *writePtr		= FillSetFromVector(	write,		inWriteSet,		nfds);
	fd_set *exceptPtr	= FillSetFromVector(	except,		inExceptSet,	nfds);

	int toRet = select(nfds + 1, readPtr, writePtr, exceptPtr, nullptr);

	if(toRet>0)
	{
		FillVectorFromSet(outReadSet, inReadSet, read);
		FillVectorFromSet(outWriteSet, inWriteSet, write);
		FillVectorFromSet(outExceptSet, inExceptSet, except);
	}

	return toRet;
}