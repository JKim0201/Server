#include <iostream>
//Project->Properties->Linker->Input->AdditionalDependency->wsock32.lib
#include <winsock.h>
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
	SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	cout << Socket << endl;
	printSocketResult(Socket);

	sockaddr_in SocketAddress = configureSocketAddress();

	int socketOptionBuffer = 0;
	if (setsockopt(Socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&socketOptionBuffer, sizeof(socketOptionBuffer) == SOCKET_ERROR))
	{
		cout << "setsockopt failed" << endl;
		WSACleanup();
		closesocket(Socket);
		exit(EXIT_FAILURE);
	}
	else
		cout << "setsockopt succeed: " << socketOptionBuffer << endl;


	int BindResult = bind(Socket, (sockaddr*)&SocketAddress, sizeof(sockaddr));
	printBindResult(BindResult);

	int ListenResult = listen(Socket, MAX_CLIENT);
	printListenResult(ListenResult);
	
	/*SOCKET Clients[MAX_CLIENT];
	int clientCount = 0;
	ZeroMemory(Clients, sizeof(SOCKET) * MAX_CLIENT);

	fd_set FDRead, FDWrite, FDExcept;
	
	FD_ZERO(&FDRead);
	FD_ZERO(&FDWrite);
	FD_ZERO(&FDExcept);

	FD_SET(Socket, &FDRead);
	FD_SET(Socket, &FDExcept);*/

	

	fd_set FDReadConnect;
	fd_set FDExceptConnect;
	FD_ZERO(&FDReadConnect);
	FD_ZERO(&FDExceptConnect);
	
	fd_set FDReadClient;
	FD_ZERO(&FDReadClient);
	
	while (true)
	{
		if (FDReadClient.fd_count < MAX_CLIENT)
		{
			FD_SET(Socket, &FDReadConnect);
			FD_SET(Socket, &FDExceptConnect);
		}

		if (FD_ISSET(Socket, &FDReadConnect))
		{
			timeval timeOut;
			timeOut.tv_sec = 1;
			timeOut.tv_usec = 0;
			int selectResult = select(FDReadConnect.fd_count + FDExceptConnect.fd_count, &FDReadConnect, nullptr, &FDExceptConnect, &timeOut);
			if (selectResult == SOCKET_ERROR)
			{
				cout << "Failed to select" << endl;
				WSACleanup();
				exit(EXIT_FAILURE);
			}
			else if (selectResult == 0)
			{
				cout << "Nothing on Port: " << PORT << endl;
			}
			else
			{
				if (FDReadClient.fd_count > MAX_CLIENT - 1)
				{
					FD_ZERO(&FDReadConnect);
					FD_ZERO(&FDExceptConnect);
				}
				// when someone connects or communicates with a message over a dedicated connection
				cout << "Data on port" << endl;
				sockaddr_in ClientAddress;
				int ClientAddressLength = sizeof(sockaddr_in);
				int ClientSocket = accept(Socket, (sockaddr*)&ClientAddress, &ClientAddressLength);

				if (ClientSocket == INVALID_SOCKET)
				{
					cout << "Failed to Accept, error code: " << WSAGetLastError() << endl;
					WSACleanup();
					exit(EXIT_FAILURE);
				}
				else
				{
					FD_SET(ClientSocket, &FDReadClient);
					cout << "client's family: " << ClientAddress.sin_family << endl;//" address: " << ClientAddress.sin_data << endl;
				}
			}
			
		}
	}

	//FD_READ

	//fd_set FDRead, FDWrite, FDExcept;

	//int maxFD = Socket;
	//timeval TimeOut;
	//TimeOut.tv_sec = 1;
	//TimeOut.tv_usec = 0;

	//int Clients[MAX_CLIENT];
	//int ClientsCount = 0;
	//ZeroMemory(Clients, sizeof(int) * MAX_CLIENT);
	//while (true)
	//{
	//	FD_ZERO(&FDRead);
	//	FD_ZERO(&FDWrite);
	//	FD_ZERO(&FDExcept);

	//	FD_SET(Socket, &FDRead);
	//	FD_SET(Socket, &FDExcept);

	//	int selectResult = select(Socket + 1, &FDRead, &FDWrite, &FDExcept, &TimeOut);

	//	if (selectResult == SOCKET_ERROR)
	//	{
	//		cout << "Failed to select" << endl;
	//		WSACleanup();
	//		exit(EXIT_FAILURE);
	//	}
	//	else if (selectResult == 0)
	//	{
	//		//time limit expire
	//		cout << "Nothing on Port: " << PORT << endl;
	//	}
	//	else
	//	{
	//		// when someone connects or communicates with a message over a dedicated connection
	//		cout << "Data on port" << endl;
	//	}
	//}

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
