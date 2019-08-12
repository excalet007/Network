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