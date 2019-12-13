#include "communicator/connection.h"
#include "shareddataaccess/gamedataaccess.h"
#include "shareddataaccess/boarddataaccess.h"
#include "communicator/prolog.h"
#include "process/process.h"

typedef struct _Match {
    BoardSHM* boardSHM;
    GameDataSHM* gameSHM;
    //TODO: remove moveTime
    int moveTime;
} Match;

int preForkHandler(Connection* connection, InputParams* inputParams, InitialSharedData* initSharedDataOut);
int communicatorEntry(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM);
int thinkerEntry(BoardSHM* boardSHM, GameDataSHM* gameSHM);

Match* initMatch(int argc, char *argv[],Connection* connection);
void teardownConnection(Connection* connection);
void teardownSHM(BoardSHM* boardSHM, GameDataSHM* gameSHM);
void initGameSHM(GameDataSHM* gameSHM, GameInstance* gameInstance);
int thinkerProcess(BoardSHM* boardSHM,GameDataSHM* gameSHM, ProcessInfo* procInfo);
int communicatorProcess(BoardSHM* boardSHM, GameDataSHM* gameSHM, Connection* connection, int moveTime, ProcessInfo* procInfo);

int initInputParams(int argc,char* argv[],InputParams* inputParams);
void deinitInputParams(InputParams* inputParams);
char* getConfigPath();