#pragma once
#include <winsock.h>

class WSA
{
public:
	WSA();
	~WSA();

	const bool isRunning(void);
private:
	WSADATA wsa;
	int status;
};