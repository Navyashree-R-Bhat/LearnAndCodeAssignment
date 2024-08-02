#include<iostream>

#include "client.h"
#include "socket.h"

int main() 
{
    SocketConnection socket(8080);
    Client client(socket);
    client.start();
	return 0; 
}