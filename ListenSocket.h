#pragma once
#include <winsock.h>



class ListenSocket
{
public:
	ListenSocket();
	~ListenSocket();

	enum class LSSTATUS
	{
		GOOD,
		BAD
	};

	const bool isListening(void);
	const SOCKET getSocket(void);
private:
	SOCKET soc;
	u_short port;
	sockaddr_in addr;
	LSSTATUS status;

	const LSSTATUS initialize(void);
};