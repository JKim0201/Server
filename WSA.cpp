#include "WSA.h"


WSA::WSA()
{
	status = WSAStartup(MAKEWORD(2, 2), &wsa);
}

WSA::~WSA()
{
	WSACleanup();
}

const bool WSA::isRunning(void)
{
	return status == 0 ? true : false;
}
