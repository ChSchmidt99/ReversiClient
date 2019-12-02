#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <stdbool.h>
#include "communicator/connection.h"
#include "communicator/servermessage.h"

//TODO: Maybe use communicator struct instead of connection
ServerMessage* getServerGreeting(Connection* connection);
ServerMessage* getVersionResponse(Connection* connection);
ServerMessage* getGameKind(Connection* connection);
ServerMessage* getGameName(Connection* connection);
ServerMessage* getPlayerMeta(Connection* connection);
int getTotalPlayers(Connection* connection);
ServerMessage* getEndplayers(Connection* connection);
char** getOtherPlayers(Connection* connection, int n);

void sendClientVersion(Connection* connection, const char* version);
void sendGameId(Connection* connection, const char* gameID);
void sendPlayerPreference(Connection* connection, const char* preference);

void formatAndSend(Connection* connection, char* data, const char* firstParam, const char* secondParam, bool freeData);

void send(Connection* connection, char* data, bool freeData);

ServerMessage* receive(Connection* connection);

#endif 