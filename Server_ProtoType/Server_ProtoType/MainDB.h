#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <mysql.h>

class MainDB
{
private:
	MYSQL* _conn, _connection;
	MYSQL_RES* result;
	MYSQL_ROW row;

	const char* _dbHost;
	const char* _dbUser;
	const char* _dbPass;
	const char* _dbName;
	char _sql[1024];

public:
	void InitDB();
	int EnrollStudentInfo(int schoolID, int group, int grade, int number);
	int SearchUUID(int schoolID, int group, int grade, int number);

};

