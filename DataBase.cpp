/*NOTE
* when to close socket?
*/

#include <iostream>
//Project->Properties->Linker->Input->AdditionalDependency->wsock32.lib
#include <winsock.h>
#include "WSA.h"
#include "ListenSocket.h"
#include <vector>

using namespace std;

int main()
{
	WSA wsa;
	if (!wsa.isRunning())
		return -1;

	ListenSocket listenSocket;
	if (!listenSocket.canListen())
		return -1;

	const int MAX_CLIENT = 5;
	vector<SOCKET> clientSockets;
	
	while (true)
	{
		if (clientSockets.size() < MAX_CLIENT)
		{
			SOCKET clientSocket = accept(listenSocket.getSocket(), NULL, NULL);
			if (clientSocket == INVALID_SOCKET) {
				switch (WSAGetLastError())
				{
				case WSAEWOULDBLOCK:
					break;
				default:
					return -1;
				}
			}
			else
			{
				clientSockets.push_back(clientSocket);
			}
		}

		for (int i = 0; i < clientSockets.size(); i++)
		{
			char buffer[256];
			int recvResult = recv(clientSockets[i], buffer, 256, 0);
			if (recvResult == SOCKET_ERROR)
			{
				cout << "recv error, need action" << endl;
			}
			else if (recvResult == 0)
			{
				cout << "connection closed" << endl;
				clientSockets.erase(clientSockets.begin() + i);
			}
			else
			{
				cout << buffer << endl;
			}
		}

		cout << "looping" << endl;
		// send/recieve

	}
	
	return 0;
}

