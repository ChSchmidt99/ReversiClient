#include "process/process.h"


Connection* initiateConnectionSequence(char* hostName, char* portNumber);
GameDataSHM* initGameDataSHM(InitialSharedData* initData);
int pipeAndFork(ProcessManagementInput* input, Connection* connection, GameDataSHM* gameSHM, BoardSHM* boardSHM);

void setupCommunicatorProcess(int pipe[2], GameDataSHM* gameSHM);
void teardownCommunicatorProcess(Connection* connection, GameDataSHM* gameSHM, BoardSHM* boardSHM);

void setupThinkerProcess(int pipe[2], GameDataSHM* gameSHM, Connection* connection);
void teardownThinkerProcess(GameDataSHM* gameSHM, BoardSHM* boardSHM);

int teardownConnection(Connection* connection);

void deinitSharedData(InitialSharedData *initialSharedData);