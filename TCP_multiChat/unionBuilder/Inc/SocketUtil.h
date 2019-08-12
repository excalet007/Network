class SocketUtil
{
public:
	static bool	StaticInit();
	static void	CleanUp();

	static void	ReportError(const char* inOperationDesc);
	static int		GetLastError();

//	static TCPSocketPtr	CreateTCPSocket(SocketAddressFamily inFamily);
};