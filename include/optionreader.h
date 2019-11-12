#ifndef OPTIONREADER_H
#define OPTIONREADER_H

#define GAME_ID_LENGTH 13
#define PLAYER_PREFERENCE_LENGTH 1

char* readGameID(int argc, char *argv[]);
char* readPreferencedPlayerNumber(int argc, char *argv[]);

#endif