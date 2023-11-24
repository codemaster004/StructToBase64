//
// Created by Filip Dabkowski on 24/11/2023.
//

#ifndef STRUCTTOBASE64_CONFIG_H
#define STRUCTTOBASE64_CONFIG_H

#define PAWNS_PER_POINT 6
#define PAWNS_PER_PLAYER 15
#define N_PLAYERS 2
#define N_DICES 2
#define MAX_NAME_LENGTH 16
#define N_BOARDS 2
#define POINTS_PER_BOARD 6

const int totalPawns = PAWNS_PER_PLAYER * N_PLAYERS;
const int nPoints = N_BOARDS * POINTS_PER_BOARD * 2;

#endif //STRUCTTOBASE64_CONFIG_H
