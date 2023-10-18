#pragma once
#include <winsock.h>
class ListenSocket
{
public:
	ListenSocket();
	~ListenSocket();
	
	const bool canListen();
	const SOCKET getSocket();
private:
	SOCKET soc;
	u_short port;
	sockaddr_in addr;
	int optionStatus;
	int ioctrlStatus;
	int bindStatus;
	int listenStatus;
};