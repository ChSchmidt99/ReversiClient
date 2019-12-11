#ifndef GAMESEQUENCE_H
#define GAMESEQUENCE_H

    #include "shareddataaccess/boarddataaccess.h"
    #include "shareddataaccess/gamedataaccess.h"
    #include "communicator/connection.h"

    int startGameLoop(Connection* connection, BoardSHM* boardSHM, GameDataSHM* gameSHM, int moveTime, int pipeReadFD);

#endif