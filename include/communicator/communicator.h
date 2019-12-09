#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <stdbool.h>
#include "communicator/connection.h"
#include "communicator/servermessage.h"

typedef struct _PlayerMeta {
    int number;
    char* name;
} PlayerMeta;

//TODO: Maybe use communicator struct instead of connection
char* getServerGreeting(Connection* connection);
char* getVersionResponse(Connection* connection);
char* getGameKind(Connection* connection);
char* getGameName(Connection* connection);
PlayerMeta* getPlayerMeta(Connection* connection);
int getTotalPlayers(Connection* connection);
char* getOtherPlayer(Connection* connection);
int nextMessageIsEndplayers(Connection* connection);

void freePlayerMeta(PlayerMeta* meta);

void sendClientVersion(Connection* connection, const char* version);
void sendGameId(Connection* connection, const char* gameID);
void sendPlayerPreference(Connection* connection, const char* preference);

void formatAndSend(Connection* connection, char* data, const char* firstParam, const char* secondParam, bool freeData);

void send(Connection* connection, char* data, bool freeData);

ServerMessage* receiveServerMessage(Connection* connection);

#endif 