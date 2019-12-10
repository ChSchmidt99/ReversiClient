#ifndef GAMESEQUENCE_H
#define GAMESEQUENCE_H

    #include "shareddataaccess/gamedataaccess.h"
    #include "communicator/connection.h"

    void startGameLoop(Connection* connection, GameDataSHM* sharedMem);

#endif