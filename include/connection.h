#include <stdlib.h>

typedef struct _Connection Connection;

Connection* newConnection(const char* hostname, const char* port);
void freeConnection(Connection* connection);

void connectToServer(Connection* connection);
void disconnectFromServer(Connection* connectoion);

char* readServerMessage(Connection* connection);
void writeServerMessage(Connection* connection, const char* message, size_t messageLength);