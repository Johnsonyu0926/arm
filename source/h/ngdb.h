/*
 * ngdb.h 
 * netguard database interface head file for oracle.
 * billy shi
 * 2003/05/04
 */

#ifndef __NG_DB_H__
#define __NG_DB_H__
#include "doorsbase.h"
class IOracleReader:public CDsObject
{
public:
	
	//Virtual interface for all other class.

	virtual int ConnectToOracle(char* szUserName,char* szPassword) = 0;
	//return 1 success,0 failed.

	virtual void Disconnect() = 0;

	virtual int ExecSQL(char* szSQL) = 0;
};

class COracleReader : public IOracleReader
{
public:
	COracleReader(){}
	virtual ~COracleReader() {}
public:
	virtual int ConnectToOracle(char* szUserName,char* szPassword);
	virtual void Disconnect();
	virtual int ExecSQL(char* szSQL);
/* Define a buffer to hold longjmp state info. */
public:
	//jmp_buf jmp_continue;
	void SqlError();

};
class IForbidClient:public CDsObject
{
public:

	virtual char* GetClientIp() = 0;
};
class IForbidClientReader:public COracleReader
{
public:
	virtual void OpenRecordset() = 0;
	virtual void Close() = 0;
	virtual IForbidClient* ReadNextClient() = 0;		
	virtual int IsEmpty() = 0;
};
class IForbidUrl:public CDsObject
{
public:
	virtual char* GetUrl() = 0;
};
class IForbidUrlReader:public COracleReader
{
public:
	virtual void OpenRecordset() = 0;
	virtual void Close() = 0;
	virtual IForbidUrl* ReadNextUrl() = 0;
};
class IForbidContent:public CDsObject
{
public:
	virtual char* GetContent() = 0;
};
class IForbidContentReader:public COracleReader
{
public:
	virtual void OpenRecordset() = 0;
	virtual void Close() = 0;
	virtual IForbidContent* ReadNextContent() = 0;
};
class IRegClient:public CDsObject
{
public:
	virtual char* GetClient() = 0;
};
class IRegClientReader:public COracleReader
{
public:
	virtual void OpenRecordset() = 0;
	virtual void Close() = 0;
	virtual IRegClient* ReadNextClient() = 0;
};
class IDatabaseLoader: public CDsObject
{
public:
	virtual BOOL Init() = 0;
	virtual BOOL ExecSQL(char* szSql) = 0;
	virtual CDsList* LoadUrl() = 0;
	virtual CDsList* LoadContent() = 0;
	virtual CDsList* LoadClientIp() = 0;	//This is the ip which should be forbided.
	virtual CDsList* LoadRegClient() = 0;	//This is the ip which has registed.
};
extern "C"
{
	int DsCreateForbidClientReader(IForbidClientReader** ppReader);
}
extern "C"
{
	int DsCreateForbidUrlReader(IForbidUrlReader** ppReader);
}
extern "C"
{
	int DsCreateForbidContentReader(IForbidContentReader** ppReader);
}
extern "C"
{
	int DsCreateRegClientReader(IRegClientReader** ppReader);
}
extern "C"
{
	int DsCreateDatabaseLoader(IDatabaseLoader** ppILoader);
}
/*
extern "C" 
{ 
	int ExecSQL(char* szSQL);
}*/
#endif
