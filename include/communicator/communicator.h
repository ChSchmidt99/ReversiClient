#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <stdbool.h>
#include "communicator/connection.h"
#include "communicator/servermessage.h"
#include "core.h"

//TODO: Maybe use communicator struct instead of connection
char* receiveServerGreeting(Connection* connection);
int hasAcceptedVersion(Connection* connection);
GameKind receiveGameKind(Connection* connection);
char* receiveGameName(Connection* connection);
PlayerMeta* receivePlayerMeta(Connection* connection);
int reveiveTotalPlayers(Connection* connection);
PlayerMeta* receiveOtherPlayer(Connection* connection);

int player1Won(Connection* connection);
int player0Won(Connection* connection);

int nextMessageIsEndplayers(Connection* connection);

char** receiveBoard(Connection* connection, size_t rows);
int receiveBoardDimensions(Connection* connection, size_t *rows, size_t *cols);

int receiveOkThink(Connection* connection);

int waitForMove(Connection* connection);
int waitForFirstMove(Connection* connection);

void freePlayerMeta(PlayerMeta* meta);

void sendMove(Connection* connection, char* move);
void sendThinking(Connection* connection);
void sendOkWait(Connection* connection);
void sendClientVersion(Connection* connection, const char* version);
void sendGameId(Connection* connection, const char* gameID);
void sendPlayerPreference(Connection* connection, const char* preference);
#endif 