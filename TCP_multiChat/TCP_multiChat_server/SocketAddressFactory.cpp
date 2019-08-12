#include "masterHeader.h"
#include <minwinbase.h>

SocketAddressPtr SocketAddressFactory::CreateIPv4FromString(const string& inString)
{
	auto pos = inString.find_last_of(':');
	string host, service;
	if(pos != string::npos)
	{
		host = inString.substr(0, pos);
		service = inString.substr(pos + 1);
	}
	else
	{
		host = inString;
		service = "0";
	}

	addrinfo hint;
	ZeroMemory(&hint, sizeof(addrinfo));
	hint.ai_family = AF_INET;
	
	addrinfo* result = nullptr;
	int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);
	addrinfo* resultHead = result;

	//error but have result case
	if(error != 0 && result != nullptr)
	{
		freeaddrinfo(resultHead);
		return nullptr;
	}

	//find first able ai_addr
	while(!result->ai_addr && result->ai_next)
	{
		result = result->ai_next;
	}

	if(!result->ai_addr)
	{
		freeaddrinfo(resultHead);
		return nullptr;
	}

	auto toRet = std::make_shared<SocketAddress>(*result->ai_addr);

	freeaddrinfo(resultHead);
	return toRet;
}
