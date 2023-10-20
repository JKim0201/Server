#pragma once
#include <winsock2.h>
#include <vector>

class SQL;

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
	void acceptClients(SQL* sql);
private:
	SOCKET soc;
	const int backlog;
	sockaddr_in addr;
	LSSTATUS status;
	const int MAX_CLIENT;
	std::vector<SOCKET> clientSockets;

	const LSSTATUS initialize(void);
};