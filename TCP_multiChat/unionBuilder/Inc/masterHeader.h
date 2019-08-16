#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#pragma comment(lib, "ws2_32.lib")

#include <minwinbase.h> //ZeroMemory
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using std::string;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;

using std::vector;
using std::shared_ptr;

const int		DEFAULT_MAX_BUF_LEN			=	1024;

const string	DEFAULT_IP_LOCAL					=	"127.0.0.1:54000";
const string	DEFAULT_IP_LOCAL_HOST		=	"127.0.0.1";
const int		DEFAULT_IP_LOCAL_SERVICE	=	54000;

#include "SocketAddress.h"
#include "SocketAddressFactory.h"

//#include "UDPSocket.h"
#include "TCPSocket.h"
	
#include "SocketUtil.h"