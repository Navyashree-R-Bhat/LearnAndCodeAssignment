#pragma once

#include <mysql_driver.h>
#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

class DatabaseConnection
{
public:
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *connection;
    sql::Statement *statement;
    sql::ResultSet *result;
    
    void connectDB();

};