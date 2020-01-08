#include "communicator/connection.h"
#include "shareddataaccess/gamedataaccess.h"
#include "shareddataaccess/boarddataaccess.h"
#include "communicator/prolog.h"
#include "process/process.h"

int preForkHandler(Connection* connection, InputParams* inputParams, InitialSharedData* initSharedDataOut);
int communicatorEntry(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM);
int thinkerEntry(BoardSHM* boardSHM, GameDataSHM* gameSHM);

GameInstance* executeProlog(Connection* connection, InputParams* params);

int initInputParams(int argc,char* argv[],InputParams* inputParams);
void deinitInputParams(InputParams* inputParams);
char* getConfigPath();