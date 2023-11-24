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
	Player *owner;
	int id;
	bool isHome;
	bool isOnBar;
	PawnColor color;
	short moveDirection;
} Pawn;

typedef struct {
	int pawnsInside;
	Pawn *pawns[PAWNS_PER_POINT];
} Point;

typedef struct {
	int pawnsInside;
	Pawn *pawns[totalPawns];
} Bar;

typedef struct {
	int pawnsInside;
	Pawn *pawns[PAWNS_PER_PLAYER];
	Player *owner;
} Court;

typedef struct {
	Pawn pawns[PAWNS_PER_PLAYER];
	Player *owner;
} PlayerPawns;

typedef struct {
	Player players[N_PLAYERS];
	PlayerPawns pawnGroups[N_PLAYERS];
	int currentPlayerId;
	Point points[nPoints];
	Court courts[N_PLAYERS];
	Bar bar;
	int dices[N_DICES];
} Board;

#endif //STRUCTTOBASE64_STRUCTS_H
