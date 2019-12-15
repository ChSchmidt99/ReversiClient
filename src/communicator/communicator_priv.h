#include "communicator/communicator.h"
#include <string.h>
#include <stdbool.h>
#include "communicator/connection.h"
#include "misc/utilities.h"

#define ENDPLAYERS_COMMAND "+ ENDPLAYERS"
#define VERSION "VERSION"
#define GAMEID "ID"
#define PLAYERPREFERENCE "PLAYER"
#define OK_WAIT_COMMAND "OKWAIT"
#define PLAY_COMMAND "PLAY"
#define THINKING_COMMAND "THINKING"
#define WON_TRUE "Yes"
#define WON_FALSE "No"

//TODO: Only use Reversi as GAMEKIND and split up at function call
#define REVERSI_GAMEKIND "+ PLAYING Reversi"

PlayerMeta* newPlayerMeta(int number, char* name, int isReady);
PlayerMeta* parsePlayerMeta(char* message);

int freeServerMessageAndReturnInt(ServerMessage* message, int i);

char* getNameFromPlayerMetaTokens(char** tokens, size_t tokenCount);
char* getNameForOtherPlayersTokens(char** tokens, size_t tokenCount);

GameKind parseGameKind(char* message);
PlayerMeta* parseOtherPlayerMeta(char* message);