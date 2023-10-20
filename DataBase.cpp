/*NOTE
* when to close socket?
*/

#include <iostream>
//Project->Properties->Linker->Input->AdditionalDependency->wsock32.lib
#include <winsock2.h>
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
	
	while (true)
		listenSocket.acceptClients(&sql);
	
	return 0;
}

