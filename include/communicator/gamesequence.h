#ifndef GAMESEQUENCE_H
#define GAMESEQUENCE_H

    #include "shareddataaccess/boarddataaccess.h"
    #include "communicator/connection.h"

    void startGameLoop(Connection* connection, BoardSHM* boardSHM);

#endif