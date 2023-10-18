#include "ListenSocket.h"

ListenSocket::ListenSocket()
	:port(9997)
{
	soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	//addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(addr.sin_zero), 0, 8);

	int socketOptionBuffer = 0;
	optionStatus = setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, (const char*)&socketOptionBuffer, sizeof(socketOptionBuffer));

	u_long mode = 1;
	ioctrlStatus = ioctlsocket(soc, FIONBIO, &mode);

	bindStatus = bind(soc, (sockaddr*)&addr, sizeof(sockaddr));

	listenStatus = listen(soc, 5);
}

ListenSocket::~ListenSocket()
{
	closesocket(soc);
}

bool ListenSocket::canListen()
{
	if (soc == INVALID_SOCKET || optionStatus == SOCKET_ERROR || ioctrlStatus == SOCKET_ERROR || bindStatus == SOCKET_ERROR || listenStatus == SOCKET_ERROR)
		return false;

	return true;
}

SOCKET ListenSocket::getSocket()
{
	return soc;
}
