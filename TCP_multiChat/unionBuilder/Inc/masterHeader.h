#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <string>
#include <sstream>

using std::string;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;

using std::shared_ptr;

const int MAX_BUFFER_LENGTH = 1024;
const string SERVER_LOCAL_HOST_SERVICE = "127.0.0.1:54000";

#include "SocketAddress.h"
#include "SocketAddressFactory.h"

//#include "UDPSocket.h"
#include "TCPSocket.h"
	
#include "SocketUtil.h"