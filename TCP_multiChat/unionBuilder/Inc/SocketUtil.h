enum SocketAddressFamily
{
	INET		=	AF_INET,
	INET6	=	AF_INET6
};

class SocketUtil
{
public:
	static bool					StaticInit();
	static void					CleanUp();

	static void					ReportError(const char* inOperationDesc);
	static int						GetLastError();

	static int						Select(	const	vector<TCPSocketPtr>* inReadSet,
																vector<TCPSocketPtr>* outReadSet,
													const	vector<TCPSocketPtr>* inWriteSet,
																vector<TCPSocketPtr>* outWriteSet,
													const	vector<TCPSocketPtr>* inExceptSet,
																vector<TCPSocketPtr>* outExceptSet);
											
	static TCPSocketPtr		CreateTCPSocket(SocketAddressFamily inEnumFamily);

private:

	inline static fd_set*		FillSetFromVector	(fd_set& outSet, const vector<TCPSocketPtr>* inSockPtrs, int& ioNaxNfds);
	inline static void			FillVectorFromSet	(vector<TCPSocketPtr>* outSockPtrs, const vector<TCPSocketPtr>* inSockPtrs, const fd_set& inSet);
};