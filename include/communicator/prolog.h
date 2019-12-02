#ifndef PROLOG_H
#define PROLOG_H

#include "communicator/communicator.h"
#include "info/info.h"

PlayerInfo* initiateProlog(Connection* connection, const char* version, const char* gameId, const char* playerPreference);

Player* parseOwn(ServerMessage* message);
#endif