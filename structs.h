//
// Created by Filip Dabkowski on 24/11/2023.
//

#ifndef STRUCTTOBASE64_STRUCTS_H
#define STRUCTTOBASE64_STRUCTS_H

#include "config.h"

enum PawnColor {
	PAWN_WHITE,
	PAWN_BLACK,
};

typedef struct {
	int id;
	char name[MAX_NAME_LENGTH];
	bool isHisTurn;
	int points;
} Player;

typedef struct {
	int ownerId;
	int id;
	bool isHome;
	bool isOnBar;
	PawnColor color;
	short moveDirection;
} Pawn;

typedef struct {
	int pawnsInside;
	int pawnsId[PAWNS_PER_POINT];
} Point;

typedef struct {
	int pawnsInside;
	int pawnsId[totalPawns];
} Bar;

typedef struct {
	int pawnsInside;
	int pawnsId[PAWNS_PER_PLAYER];
	int ownerId;
} Court;

typedef struct {
	Pawn pawns[totalPawns];
	Player players[N_PLAYERS];
	int currentPlayerId;
	Point points[nPoints];
	Court courts[N_PLAYERS];
	Bar bar;
	int dices[N_DICES];
} Board;

#endif //STRUCTTOBASE64_STRUCTS_H
