#include <sys/socket.h>
#include <sys/ioctl.h>
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

void disconnectFromServer(Connection* connection){
    if (connection->socket != -1){
        if (close(connection->socket) == -1)
            die("Failed To close Socket");
    }
    connection->socket = -1;
}

char* readServerMessage(Connection* connection){
    if (connection->socket == -1)
        die("Not connectet to server");
    
    //TODO: Get buff size dynamically
    char* buffer = malloc(sizeof(char) * BUFFSIZE);
    ssize_t len = read(connection->socket, buffer, BUFFSIZE - 1);
    buffer[len] = '\0';
    return buffer;
}

void writeMessageToServer(Connection* connection, char* message){
    if (connection->socket == -1)
        die("Not connectet to server");
    
    char* in = concatStringToNewMemoryAddr(message,"\n","");
    if (write(connection->socket, in, strlen(in)) == -1)
        die("Failed to write message to server");
    free(in);
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