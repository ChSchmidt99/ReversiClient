#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#include "../include/utilities.h"
#include "../include/connection.h"

#define BUFFSIZE 256

struct addrinfo* getSocketAddr(const char* hostname, const char* port);

struct _Connection {
    char* hostname;
    char* port;
    int socket;
};

Connection* newConnection(const char* hostname, const char* port){
    Connection* connection = malloc(sizeof(Connection));
    connection->hostname = copyStringToNewMemoryAddr(hostname);
    connection->port = copyStringToNewMemoryAddr(port);
    connection->socket = -1;
    return connection;
}

void freeConnection(Connection* connection) {
    free(connection->hostname);
    free(connection->port);
    free(connection);
}

//TODO: Pass errors to instance above
void connectToServer(Connection* connection){    
    struct addrinfo* socketAddr = getSocketAddr(connection->hostname, connection->port);
    if (socketAddr == NULL)
        die("failed to get address information");

    int sock = socket(socketAddr->ai_family,socketAddr->ai_socktype,socketAddr->ai_protocol);
    if (sock == -1) 
        die("failed to create socket");
        

    if (connect(sock,socketAddr->ai_addr,socketAddr->ai_addrlen) == -1) 
        die("failed to connect to socket");
        
    freeaddrinfo(socketAddr);
    connection->socket = sock;
}

//TODO: Pass errors to instance above
void disconnectFromServer(Connection* connection){
    if (connection->socket != -1){
        if (close(connection->socket) == -1)
            die("Failed To close Socket");
    }
    connection->socket = -1;
}

//TODO: Pass errors to instance above
char* readServerMessage(Connection* connection){
    if (connection->socket == -1)
        die("Not connectet to server");
    
    char* message = malloc(sizeof(char) * BUFFSIZE);
    read(connection->socket, message, BUFFSIZE);
    return message;
}

//TODO: Pass errors to instance above
void writeServerMessage(Connection* connection, const char* message, size_t messageLength){
    if (connection->socket == -1)
        die("Not connectet to server");
    
    if (write(connection->socket, message, BUFFSIZE) == -1)
        die("Failed to write message to server");
}

struct addrinfo* getSocketAddr(const char* hostname, const char* port) {
    struct addrinfo hints;
    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_ADDRCONFIG;
    struct addrinfo* res = NULL;
    int err = getaddrinfo(hostname,port,&hints,&res);
    if (err != 0) {
        freeaddrinfo(res);
        return NULL;
    }
    return res;
}