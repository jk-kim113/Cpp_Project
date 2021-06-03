#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <mysql.h>

int main()
{
    std::cout << "Hello World!\n";

    MYSQL* conn, connection;
    MYSQL_RES* result;
    MYSQL_ROW row;

    char DB_HOST[] = "127.0.0.1";
    char DB_USER[] = "root";
    char DB_PASS[] = "1234";
    char DB_NAME[] = "testdb";
    char sql[1024];
    char str1[1024], str2[1024];

    mysql_init(&connection);
    conn = mysql_real_connect(&connection, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);

    if (conn == NULL)
    {
        fprintf(stderr, "Mysql connection error : %s", mysql_error(&connection));
        return 1;
    }

    /*strcpy(sql, "INSERT INTO testtable(UUID, NickName) VALUES(100, 'test')");

    if (mysql_query(conn, sql) != 0)
    {
        printf("Error\n");
    }*/

    strcpy(sql, "select * from testtable");
    if (mysql_query(conn, sql) != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&connection));
    }
    result = mysql_store_result(conn);
    while ((row = mysql_fetch_row(result)) != NULL)
    {
        printf("%s %s \n", row[0], row[1]);
    }

    mysql_close(conn);
}