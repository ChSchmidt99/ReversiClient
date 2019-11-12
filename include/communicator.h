#include "./connection.h"
#include "./list.h"

//TODO: Maybe use communicator struct instead of connection
char*  getServerGreeting(Connection* connection);
char*  getVersionResponse(Connection* connection);
char*  getGameKind(Connection* connection);
char*  getGameName(Connection* connection);
char*  getPlayerMeta(Connection* connection);

// A char* List
List* getOtherPlayers(Connection* connection);

void sendClientVersion(Connection* connection, const char* version);
void sendGameId(Connection* connection, const char* gameID);
void sendPlayerPreference(Connection* connection, const char* preference);