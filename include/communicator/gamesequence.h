#ifndef GAMESEQUENCE_H
#define GAMESEQUENCE_H

    #include "shareddataaccess/shareddataaccess.h"
    #include "communicator/connection.h"

    void startGameLoop(Connection* connection, SharedMemory* sharedMem);

#endif