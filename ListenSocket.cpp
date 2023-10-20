#include "ListenSocket.h"
#include <ws2tcpip.h>
#include <string>
#include "SQL.h"


ListenSocket::ListenSocket()
	:backlog(5),
	MAX_CLIENT(5),
	status(initialize())
{
}

ListenSocket::~ListenSocket()
{
	closesocket(soc);
}

const  ListenSocket::LSSTATUS ListenSocket::initialize(void)
{
	soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (soc == INVALID_SOCKET)
		return LSSTATUS::BAD;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(9997);
	inet_pton(AF_INET, "192.168.1.66", &(addr.sin_addr));

	u_long mode = 1;
	if (SOCKET_ERROR == ioctlsocket(soc, FIONBIO, &mode))
		return LSSTATUS::BAD;

	if (SOCKET_ERROR == bind(soc, (sockaddr*)&addr, sizeof(sockaddr)))
		return LSSTATUS::BAD;

	if (SOCKET_ERROR == listen(soc, backlog))
		return LSSTATUS::BAD;

	return LSSTATUS::GOOD;
}


const bool ListenSocket::isListening(void)
{
	return status == LSSTATUS::GOOD ? true : false;
}

void ListenSocket::acceptClients(SQL* sql)
{
	if (clientSockets.size() < MAX_CLIENT)
	{
		SOCKET clientSocket = accept(soc, NULL, NULL);
		if (clientSocket == INVALID_SOCKET) {
			switch (WSAGetLastError())
			{
			case WSAEWOULDBLOCK:
				break;
			default:
				break;
			}
		}
		else
			clientSockets.push_back(clientSocket);
	}

	for (int i = 0; i < clientSockets.size(); i++)
	{
		const unsigned int BUFFER_SIZE = 256;
		char recvBuffer[BUFFER_SIZE];
		int recvResult = recv(clientSockets[i], recvBuffer, BUFFER_SIZE, 0);
		if (recvResult == SOCKET_ERROR)
		{
			//cout << "recv error, need action" << endl;
		}
		else if (recvResult == 0)
		{
			// client dropped
			clientSockets.erase(clientSockets.begin() + i);
		}
		else
		{
			const unsigned int userID = sql->authenticate(recvBuffer);
			std::string sUserID = std::to_string(userID);
			int sendResult = send(clientSockets[i], sUserID.c_str(), sUserID.length() + 1, 0);
		}
	}
}



