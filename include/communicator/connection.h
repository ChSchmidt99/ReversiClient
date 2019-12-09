#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdlib.h>

#define DEFAULT_MESSAGE_BUFFER_SIZE 512

typedef struct _Connection Connection;

Connection* newConnection(const char* hostname, const char* port);
void freeConnection(Connection* connection);

void connectToServer(Connection* connection);
void disconnectFromServer(Connection* connectoion);

//Depreciated, use readLineFromServer instead!
char* readServerMessage(Connection* connection, size_t buffSize, char buffer[buffSize]);

char* readLineFromServer(Connection* connection);
void writeLineToServer(Connection* connection, char* message);

#endif 