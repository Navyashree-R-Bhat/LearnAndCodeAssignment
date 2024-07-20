#include "dbConnection.h"

DatabaseConnection::DatabaseConnection()
{
    driver = sql::mysql::get_mysql_driver_instance();
    connection = driver->connect("tcp://127.0.0.1:3306", "navyashree", "");

    connection->setSchema("Cafeteria");
}

DatabaseConnection::~DatabaseConnection()
{
    delete connection;
}

sql::Connection* DatabaseConnection::getConnection()
{
    return connection;
}

// int main()
// {
//     DatabaseConnection db;
//     db.connectDB();
// }