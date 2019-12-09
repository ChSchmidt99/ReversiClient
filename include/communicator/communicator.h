#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <stdbool.h>
#include "communicator/connection.h"
#include "communicator/servermessage.h"
#include "core.h"

//TODO: Maybe use communicator struct instead of connection
char* getServerGreeting(Connection* connection);
int hasAcceptedVersion(Connection* connection);
GameKind getGameKind(Connection* connection);
char* getGameName(Connection* connection);
PlayerMeta* getPlayerMeta(Connection* connection);
int getTotalPlayers(Connection* connection);
PlayerMeta* getOtherPlayer(Connection* connection);
int nextMessageIsEndplayers(Connection* connection);

void freePlayerMeta(PlayerMeta* meta);

void sendClientVersion(Connection* connection, const char* version);
void sendGameId(Connection* connection, const char* gameID);
void sendPlayerPreference(Connection* connection, const char* preference);
#endif 