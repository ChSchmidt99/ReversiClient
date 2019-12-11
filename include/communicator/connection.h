#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdlib.h>
#include <sys/types.h>
#include "servermessage.h"

#define DEFAULT_MESSAGE_BUFFER_SIZE 512

typedef struct _Connection Connection;

Connection* newConnection(const char* hostname, const char* port);
void freeConnection(Connection* connection);

int connectToServer(Connection* connection);
int disconnectFromServer(Connection* connectoion);

ServerMessage* receiveServerMessage(Connection* connection);

//Depreciated, use readLineFromServer instead!
char* readServerMessage(Connection* connection, ssize_t buffSize, char buffer[buffSize]);

char* readLineFromServer(Connection* connection);
void writeLineToServer(Connection* connection, char* message);

#endif 