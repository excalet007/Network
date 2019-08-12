class SocketAddress;
typedef shared_ptr< SocketAddress > SocketAddressPtr;

class SocketAddress
{
public:
	//default constructor
	SocketAddress()
	{
		GetAsSockAddrIn()->sin_family = AF_INET;
		GetAsSockAddrIn()->sin_addr.S_un.S_addr = INADDR_ANY;
		GetAsSockAddrIn()->sin_port = 0;
	}

	//specific address, port constructor
	SocketAddress(uint32_t inAddress, uint16_t inPort)
	{
		GetAsSockAddrIn()->sin_family = AF_INET;
		GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(inAddress);
		GetAsSockAddrIn()->sin_port = htons(inPort);
	}

	//SocketAddressFactory use this
	SocketAddress(const sockaddr& inSockAddr)
	{
		memcpy(&mSockAddr, &inSockAddr, sizeof(sockaddr));
	}

	size_t GetSize() const { return sizeof(sockaddr); }

private:
	friend class TCPSocket;
	friend class UDPSocket;

	sockaddr mSockAddr;
	sockaddr_in* GetAsSockAddrIn()
	{
		return reinterpret_cast<sockaddr_in*>(&mSockAddr);
	}
};
