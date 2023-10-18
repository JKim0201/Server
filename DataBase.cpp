/*NOTE
* when to close socket?
*/

#include <iostream>
//Project->Properties->Linker->Input->AdditionalDependency->wsock32.lib
#include <winsock.h>
#include "WSA.h"
#include "ListenSocket.h"

using namespace std;

int main()
{
	WSA wsa;
	if (!wsa.isRunning())
		return -1;

	ListenSocket listenSocket;
	if (!listenSocket.canListen())
		return -1;


	while (true)
	{
		SOCKET AcceptSocket;
		AcceptSocket = accept(listenSocket.getSocket(), NULL, NULL);
		if (AcceptSocket == INVALID_SOCKET) {
			wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
			switch (WSAGetLastError())
			{
				case WSAEWOULDBLOCK:
				{
					cout << "nothing in queue" << endl;
					break;
				}
				default:
				{
					closesocket(listenSocket.getSocket());
					WSACleanup();
					exit(EXIT_FAILURE);
					break;
				}
			}	
		}
		else
			wprintf(L"Client connected.\n");


		cout << "doing stuff" << endl;
	}
	
	return 0;
}

