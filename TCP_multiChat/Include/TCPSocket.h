class TCPSocket;
typedef shared_ptr<TCPSocket> TCPSocketPtr;


//TODO WHY accept parameter is not const reference? int32_t to uint32_t able?
class TCPSocket
{
public:
	~TCPSocket();
	int										Connect	(const SocketAddress& inAddress);
	int										Bind			(const SocketAddress& inAddress);
	int										Listen		(int inBackLog = 32);
	TCPSocketPtr						Accept		(SocketAddress& inFromAddress);
	int32_t									Send		(const void* inData, size_t inLen);
	int32_t									Receive	(void* inData, size_t inLen);

private:
	friend class SocketUtil;

	//private constructor : 
	TCPSocket(SOCKET inSocket) : mSocket(inSocket){}
	SOCKET mSocket;
};