#include "SQL.h"
#include <sql.h>
#include <sqlext.h>
#include <string>

using namespace std;

SQL::SQL()
	:status(initialize())
{
}

SQL::~SQL()
{
	SQLFreeHandle(SQL_HANDLE_STMT, statementHandle);
	SQLDisconnect(connectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, connectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, environmentHandle);
}

SQL::SQLSTATUS SQL::initialize()
{
	
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &environmentHandle))
		return SQLSTATUS::BAD;
	if (SQL_SUCCESS != SQLSetEnvAttr(environmentHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
		return SQLSTATUS::BAD;

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, environmentHandle, &connectionHandle))
		return SQLSTATUS::BAD;

	if (SQL_SUCCESS != SQLSetConnectAttr(connectionHandle, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0))
		return SQLSTATUS::BAD;

	int driverConnectResult = SQLDriverConnect(connectionHandle, NULL, (SQLWCHAR*)L"DRIVER={SQL Server}; SERVER=localhost, 1433; DATABASE=Test; UID=testID; PWD=myPasword;", SQL_NTS, nullptr, 0, NULL, SQL_DRIVER_NOPROMPT);
	if (SQL_SUCCESS != driverConnectResult && SQL_SUCCESS_WITH_INFO != driverConnectResult)
		return SQLSTATUS::BAD;

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, connectionHandle, &statementHandle))
		return SQLSTATUS::BAD;

	return SQLSTATUS::GOOD;
}

const bool SQL::isConnected(void)
{
	return status == SQLSTATUS::GOOD ? true : false;
}

const unsigned int SQL::authenticate(const char* data) const
{
	string emailPassword = data;
	const char DELIMITER_AT = emailPassword.find('/');
	string email = emailPassword.substr(0, DELIMITER_AT);
	string password = emailPassword.substr(DELIMITER_AT + 1, emailPassword.length() - 1);
	wstring wEmail(email.begin(), email.end());
	wstring wPassword(password.begin(), password.end());
	wstring wQuery = L"SELECT UserID FROM USERS WHERE Email = \'" + wEmail + L"\' AND Password = '" + wPassword + L"\'";

	unsigned int userID = 0;
	int execDirectResult = SQLExecDirect(statementHandle, (SQLWCHAR*)(wQuery.c_str()), SQL_NTS);
	switch (execDirectResult)
	{
	case SQL_SUCCESS:
		break;
	case SQL_SUCCESS_WITH_INFO:
		break;
	case SQL_ERROR:
		SQLWCHAR SQLState[1024];
		SQLWCHAR message[1024];
		SQLGetDiagRec(SQL_HANDLE_STMT, statementHandle, 1, SQLState, NULL, message, 1024, NULL);
		wprintf(L"SQL driver message: \n");
		wprintf((SQLWCHAR*)message);
		wprintf(L"SQL driver state: \n");
		wprintf((SQLWCHAR*)SQLState);
		return userID;
	default:
		return userID;
	}

	while (SQL_SUCCESS == SQLFetch(statementHandle))
	{
		SQLGetData(statementHandle, 1, SQL_C_DEFAULT, &userID, sizeof(userID), NULL);
	}
	SQLCloseCursor(statementHandle);

	return userID;
}

