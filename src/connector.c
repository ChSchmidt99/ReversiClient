#include "../include/connector.h"
#include "../include/utilities.h"
#include "../include/servermessage.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>


#define BUFFSIZE 256

//TODO: Maybe split up in network manager and protocol manager

struct _Connector {
    char* hostname;
    char* port;
    int socket;
};

Connector* newConnector(const char* hostname, const char* port) {
    Connector* connector = malloc(sizeof(Connector));
    connector->hostname = copyStringToNewMemoryAddr(hostname);
    connector->port = copyStringToNewMemoryAddr(port);
    connector->socket = -1;
    return connector;
}

void freeConnector(Connector* connector) {
    free(connector->hostname);
    free(connector->port);
    free(connector);
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

void connectToServer(Connector* connector){    
    struct addrinfo* socketAddr = getSocketAddr(connector->hostname, connector->port);
    if (socketAddr == NULL)
        die("failed to get address information");

    int sock = socket(socketAddr->ai_family,socketAddr->ai_socktype,socketAddr->ai_protocol);
    if (sock == -1) 
        die("failed to create socket");
        

    if (connect(sock,socketAddr->ai_addr,socketAddr->ai_addrlen)==-1) 
        die("failed to connect to socket");
        
    freeaddrinfo(socketAddr);
    connector->socket = sock;
}

void disconnectFromServer(Connector* connector){
    if (connector->socket != -1){
        if (close(connector->socket) == -1)
            die("Failed To close Socket");
    }
    connector->socket = -1;
}

void getServerGreeting(int socket){
    char buffer[BUFFSIZE];
    ssize_t readSize = read(socket, buffer, BUFFSIZE);
    if (readSize < 0)
        die("Failed to read from socket");

    ServerMessage* message = parseServerVersion(buffer);
    if (message->type == Error)
        die(message->message);

    printServerMessage(message);
    freeServerMessage(message);
}

void sendClientVersion(int socket){
    //TODO: Add Version number to #define
    char buffer[] = "VERSION 2.0\n";
    write(socket, buffer, strlen(buffer));
}

void getVersionResponse(int socket){
    char buffer[BUFFSIZE];
    ssize_t readSize = read(socket, buffer, BUFFSIZE);
    if (readSize < 0)
        die("Failed to read from socket");

    //TODO: Implement me
    printf("%s\n",buffer);
}

void initiateProlog(Connector *connector) {
    if (connector->socket == -1) 
        die("Connector not connected to server!");

    getServerGreeting(connector->socket);
    sendClientVersion(connector->socket);
    getVersionResponse(connector->socket);
}