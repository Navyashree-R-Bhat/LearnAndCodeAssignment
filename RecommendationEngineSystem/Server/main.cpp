#include "socket.h"

int main()
{
    DatabaseConnection database;
    SocketConnection server(8080, &database);
    server.run();
    return 0;
}
