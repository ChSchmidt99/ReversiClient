#include "process/process.h"

typedef struct _ProcessInfo {
    pid_t child;
    pid_t parent;
    int fd[2];
} ProcessInfo;

Connection* initiateConnectionSequence(char* hostName, char* portNumber);
GameDataSHM* initGameDataSHM(InitialSharedData* initData);
int pipeAndFork(ProcessManagementInput* input, Connection* connection, GameDataSHM* gameSHM, BoardSHM* boardSHM);

ProcessInfo* setupCommunicatorProcess(int pipe[2], GameDataSHM* gameSHM);
void teardownCommunicatorProcess(ProcessInfo* processInfo, Connection* connection, GameDataSHM* gameSHM, BoardSHM* boardSHM);

ProcessInfo* setupThinkerProcess(int pipe[2], GameDataSHM* gameSHM, Connection* connection);
void teardownThinkerProcess(ProcessInfo* processInfo, GameDataSHM* gameSHM, BoardSHM* boardSHM);

int teardownConnection(Connection* connection);

ProcessInfo* createProcessInfo(int pipe[2]);
void freeProcessInfo(ProcessInfo* procInfo);

int readFileDescriptor(ProcessInfo* info);
int writeFileDescriptor(ProcessInfo* info);