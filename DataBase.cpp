/*NOTE
* when to close socket?
*/

#include <iostream>
//Project->Properties->Linker->Input->AdditionalDependency->wsock32.lib
#include <winsock.h>
#include <thread>
#include <mutex>
#define PORT 9997
#define MAX_CLIENT 5

using namespace std;

void startWSA();
void printWSAInfo(WSADATA& ws);
void printSocketResult(int socketResult);
sockaddr_in& configureSocketAddress();
void printBindResult(int bindResult);
void printListenResult(int listenResult);


int main()
{

	startWSA();
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	printSocketResult(listenSocket);

	sockaddr_in SocketAddress = configureSocketAddress();

	int socketOptionBuffer = 0;
	if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&socketOptionBuffer, sizeof(socketOptionBuffer) == SOCKET_ERROR))
	{
		cout << "setsockopt failed" << endl;
		WSACleanup();
		closesocket(listenSocket);
		exit(EXIT_FAILURE);
	};
	
	u_long mode = 1;
	if (ioctlsocket(listenSocket, FIONBIO, &mode) == SOCKET_ERROR)
	{
		printf("ioctlsocket failed with error: %ld\n", mode);
		WSACleanup();
		closesocket(listenSocket);
		exit(EXIT_FAILURE);
	}
	else
	{
		cout << "Mdoe: " << mode << endl;
	}



	int bindResult = bind(listenSocket, (sockaddr*)&SocketAddress, sizeof(sockaddr));
	printBindResult(bindResult);

	int listenResult = listen(listenSocket, MAX_CLIENT);
	printListenResult(listenResult);

	while (true)
	{
		SOCKET AcceptSocket;
		AcceptSocket = accept(listenSocket, NULL, NULL);
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
					closesocket(listenSocket);
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

	closesocket(listenSocket);
	WSACleanup();
}

void startWSA()
{
	//Initialize WSA
	WSADATA wsa;
	int WASStartupResult = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (WASStartupResult < 0)
	{
		cout << "WSA start up failed" << endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
	{
		cout << "WSA start up succeed" << endl;
		printWSAInfo(wsa);
	}
}

void printWSAInfo(WSADATA& ws)
{
	int VENDORINFO_FAIL = -3689348814741910324;

	cout << "Version: " << ws.wVersion << endl;
	cout << "HighVersion: " << ws.wHighVersion << endl;
	cout << "MaxSocket: " << ws.iMaxSockets << endl;
	cout << "MaxUdpDg: " << ws.iMaxUdpDg << endl;
	if (int(ws.lpVendorInfo) == VENDORINFO_FAIL)
	{
		cout << "lpVendorInfo: FAILED TO RETRIEVE" << endl;
	}
	else
	{
		int i = 0;
		cout << "lpVendorInfo: " << endl;
		while (ws.lpVendorInfo[i])
		{
			cout << ws.lpVendorInfo[i];
			i++;
		}
		cout << endl;
	}
	cout << "szDescription: " << ws.szDescription << endl;
	cout << "szSystemStatus: " << ws.szSystemStatus << endl;
}

void printSocketResult(int socketResult)
{
	if (socketResult < 0)
	{
		cout << "Socket not opened: " << socketResult << endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
		cout << "Socket opened" << endl;
}

sockaddr_in& configureSocketAddress()
{
	sockaddr_in socketAddress;
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_port = htons(PORT);
	//srv.sin_addr.s_addr = inet_addr("127.0.0.1");
	socketAddress.sin_addr.s_addr = INADDR_ANY;
	memset(&(socketAddress.sin_zero), 0, 8);
	return socketAddress;
}

void printBindResult(int bindResult)
{
	if (bindResult < 0)
	{
		cout << "failed to bind to local port" << endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
		cout << "Succeed to bind to local port" << endl;
}

void printListenResult(int listenResult)
{
	if (listenResult < 0)
	{
		cout << "failed to listen to local port" << endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
	{
		cout << "Succeed to listen to local port" << endl;
	}
}

