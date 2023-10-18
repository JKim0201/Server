#include "WSA.h"
#include <winsock.h>

WSA::WSA()
{
	status = WSAStartup(MAKEWORD(2, 2), &wsa);
}

int WSA::getStatus()
{
	return status;
}
