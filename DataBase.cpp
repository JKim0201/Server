/*NOTE
* when to close socket?
*/

#include <iostream>
//Project->Properties->Linker->Input->AdditionalDependency->wsock32.lib
#include <winsock.h>
#include "SQL.h"
#include "WSA.h"
#include "ListenSocket.h"

#include <vector>
#include <string>

using namespace std;

int main()
{
	SQL sql;
	if (!sql.isConnected())
		return -1;

	WSA wsa;
	if (!wsa.isRunning())
		return -1;

	ListenSocket listenSocket;
	if (!listenSocket.isListening())
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
			const unsigned int BUFFER_SIZE = 256;
			char recvBuffer[BUFFER_SIZE];
			int recvResult = recv(clientSockets[i], recvBuffer, BUFFER_SIZE, 0);
			if (recvResult == SOCKET_ERROR)
			{
				//cout << "recv error, need action" << endl;
			}
			else if (recvResult == 0)
			{
				cout << "connection closed" << endl;
				clientSockets.erase(clientSockets.begin() + i);
			}
			else
			{
				//don't forget to clear buffer
				// id/pwd + \0

				const unsigned int userID = sql.authenticate(recvBuffer);
				string sUserID = to_string(userID);
				int sendResult = send(clientSockets[i], sUserID.c_str(), sUserID.length() + 1, 0);
				if (sendResult == SOCKET_ERROR)
				{
					cout << "send fail:" << WSAGetLastError() << endl;
				}
				else
				{
					cout << "byte Sent : " << sendResult << endl;
				}
			}
		}

	}
	
	return 0;
}

