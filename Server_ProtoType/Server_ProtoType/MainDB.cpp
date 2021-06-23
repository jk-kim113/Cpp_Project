#include "MainDB.h"

void MainDB::InitDB()
{
	_dbHost = "127.0.0.1";
	_dbUser = "root";
	_dbPass = "1234";
	_dbName = "testdb";

	mysql_init(&_connection);
	_conn = mysql_real_connect(&_connection, _dbHost, _dbUser, _dbPass, _dbName, 3306, (char*)NULL, 0);

	if (_conn == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&_connection));
	}
}

int MainDB::EnrollStudentInfo(int schoolID, int grade, int group, int number)
{
	sprintf(_sql, "INSERT INTO studenttable(SchoolID,ClassGrade,ClassGroup,ClassNumber) VALUES(%d,%d,%d,%d)", schoolID, grade, group, number);
	if (mysql_query(_conn, _sql) != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&_connection));
		return 1;
	}

	std::cout << "학생 DB에 정보 저장 성공" << "\n";

	return 0;
}

int MainDB::SearchUUID(int schoolID, int group, int grade, int number)
{
	int myUUID = 0;
	sprintf(_sql, "SELECT UUID FROM studenttable WHERE SchoolID = %d AND ClassGroup = %d AND ClassGrade = %d AND ClassNumber = %d", schoolID, grade, group, number);

	if (mysql_query(_conn, _sql) != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&_connection));
		return -1;
	}

	result = mysql_store_result(_conn);
	while ((row = mysql_fetch_row(result)) != NULL)
	{
		myUUID = atoi(row[0]);
		std::cout << "찾은 UUID : " << row[0] <<"\n";
	}

	return myUUID;
}
