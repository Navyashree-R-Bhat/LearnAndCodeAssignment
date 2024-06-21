#include "../inc/dbConnection.h"

void DatabaseConnection::connectDB()
{
    driver = sql::mysql::get_mysql_driver_instance();
    connection = driver->connect("tcp://127.0.0.1:3306", "navyashree", "");

    connection->setSchema("Cafeteria");

    statement = connection->createStatement();
}

int main()
{
    DatabaseConnection db;
    db.connectDB();
}