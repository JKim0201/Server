#pragma once
#include <windows.h>
#include <sqltypes.h>



class SQL
{
public:
	SQL();
	~SQL();

	enum class SQLSTATUS
	{
		GOOD,
		BAD
	};

	const bool isConnected(void);
	const unsigned int authenticate(const char* data) const;
private:
	SQLHANDLE environmentHandle;
	SQLHANDLE connectionHandle;
	SQLHANDLE statementHandle;
	SQLSTATUS status;
	
	SQLSTATUS initialize(void);
};

