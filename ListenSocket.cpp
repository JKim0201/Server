#include "ListenSocket.h"

ListenSocket::ListenSocket()
	:port(9997),
	status(initialize())
{
}

ListenSocket::~ListenSocket()
{
	closesocket(soc);
}

const bool ListenSocket::isListening(void)
{
	return status == LSSTATUS::GOOD ? true : false;
}

const SOCKET ListenSocket::getSocket(void)
{
	return soc;
}

const  ListenSocket::LSSTATUS ListenSocket::initialize(void)
{
	soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (soc == INVALID_SOCKET)
		return LSSTATUS::BAD;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	//addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(addr.sin_zero), 0, 8);

	int socketOptionBuffer = 0;
	if (SOCKET_ERROR == setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, (const char*)&socketOptionBuffer, sizeof(socketOptionBuffer)))
		return LSSTATUS::BAD;

	u_long mode = 1;
	if (SOCKET_ERROR == ioctlsocket(soc, FIONBIO, &mode))
		return LSSTATUS::BAD;

	if (SOCKET_ERROR == bind(soc, (sockaddr*)&addr, sizeof(sockaddr)))
		return LSSTATUS::BAD;

	if (SOCKET_ERROR ==listen(soc, 5))
		return LSSTATUS::BAD;

	return LSSTATUS::GOOD;
}

