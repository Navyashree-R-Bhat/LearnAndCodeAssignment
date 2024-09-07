#include "server.h"

int main()
{
    DatabaseConnection database;
    Server server(8080, &database);
    server.run();
    return 0;
}
