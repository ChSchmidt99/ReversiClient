#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include "./connection.h"
#include "./servermessage.h"
#include "./list.h"

//TODO: Maybe use communicator struct instead of connection
ServerMessage* getServerGreeting(Connection* connection);
ServerMessage* getVersionResponse(Connection* connection);
ServerMessage* getGameKind(Connection* connection);
ServerMessage* getGameName(Connection* connection);
ServerMessage* getPlayerMeta(Connection* connection);

// A ServerMessage* List
List* getOtherPlayers(Connection* connection);

void sendClientVersion(Connection* connection, const char* version);
void sendGameId(Connection* connection, const char* gameID);
void sendPlayerPreference(Connection* connection, const char* preference);

#endif 