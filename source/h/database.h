/*
	mysql C++ wrapper library

	Author: Roland Haenel <rh@ginster.net>

	This program is in the public domain.
	Distribute and use it freely.
*/

#ifndef __DATABASE_H
#define __DATABASE_H

#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>		
#include <string.h>


#define DB_CONNECTION_NONE	0
#define DB_CONNECTION_OK	1
#define DB_CONNECTION_BAD	2

#define DB_COMMAND_OK		0	// OK - command executed
#define DB_EMPTY_QUERY		1	// Query didn't return tuples
#define DB_TUPLES_OK		2	// Query returned tuples
#define DB_ERROR		5
#define DB_BAD_RESPONSE		6
#define DB_UNEXPECTED		7	// This shouldn't ever happen

class DBResult;

class Database {
private:
	MYSQL* mysql;
	bool connected;
	char error[100];		// Error description
public:
	Database();
	~Database();
	
	int init();			// Initialize and do basic tests
	int status();			// Return status information
	const char *errorMessage();		// Return current error message
	
	int connect(char *host, char* user,char* passwd, char *port, char *db);	// Connect to db
	void disconnect();		// Disconnect from database
	int reset();			// Reset connection

	DBResult *exec(char *sqlFormat);	// Execute arbitrary SQL cmd
						// Same as above, reuse res
	void exec(DBResult *res, char *sqlFormat);
};

class DBResult {
private:
	bool haveError;
	MYSQL_RES *result;
public:
	DBResult();
	DBResult(MYSQL *mysql, char *query);
	~DBResult();

	void init(MYSQL *mysql, char *query);
	
	int status();			// Return query status
		
	int nrTuples();			// Number of fetched tuples
	int nrFields();			// Number of fields per tuple

	char *fieldName(int n);		// Name of nth field
	int   fieldSize(int n);		// Size of nth field
	int   fieldSize(char *name);	// Size of nth field

	void seekTuple(int tuple);      // Sets internal cursor to tuple
	char **getTuple();              // Return the next tuple or NULL
	char **getTuple(int tuple);	// Return tuple

	long GetAffectedRows();
private:
	long m_nAffectedRows;

};

#endif
