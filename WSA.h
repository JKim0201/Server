#pragma once
#include <winsock.h>

class WSA
{
public:
	WSA();
	~WSA();

	const bool isRunning();
private:
	WSADATA wsa;
	int status;
};