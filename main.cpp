#include "iostream"
#include "cstring"

#include "structs.h"

using namespace std;

Board initEverything() {
	auto board = Board{};
	for (int &dice: board.dices) {
		dice = 0;
	}
	for (Player &player: board.players) {
		player = Player{9, "Hello", false, 4};
	}
	for (PlayerPawns &group : board.pawnGroups) {
		group.owner = &board.players[0];
		for (Pawn &pawn : group.pawns) {
			pawn = Pawn {&board.players[0], 1, false, false, PAWN_BLACK, -1};
		}
	}
	for (auto &point: board.points) {
		point = Point{7};
		for (Pawn* &j: point.pawns) {
			j = nullptr;
		}
	}
	for (auto &court: board.courts) {
		court = Court{.pawnsInside=4, .owner=&board.players[1]};
		for (Pawn* &j: court.pawns) {
			j = nullptr;
		}
	}
	board.bar = Bar{5};
	for (Pawn* &pawn: board.bar.pawns) {
		pawn = nullptr;
	}
	board.bar.pawns[0] = &board.pawnGroups[0].pawns[1];
	board.currentPlayerId = 3;

	return board;
}

void serializeInt(int value, uint8_t *buffer, size_t &offset) {
	std::memcpy(buffer + offset, &value, sizeof(int));
	offset += sizeof(int);
}

int deserializeInt(const uint8_t *buffer, size_t &index) {
	int value;
	std::memcpy(&value, buffer + index, sizeof(int));
	index += sizeof(int);
	return value;
}

void serialisePawn(Pawn pawn, uint8_t *buffer, size_t &offset) {
	std::memcpy(buffer + offset, &pawn, sizeof(Pawn));
	offset += sizeof(Pawn);
}

void serialisePawnPointer(Pawn *pawn, uint8_t* buffer, size_t &offset) {
	if (pawn == nullptr){
		serializeInt(-1, buffer, offset);
	} else {
		serializeInt(pawn->id, buffer, offset);
	}
}

Pawn deserializePawn(const uint8_t *buffer, size_t &index) {
	Pawn pawn;
	std::memcpy(&pawn, buffer + index, sizeof(Pawn));
	index += sizeof(Pawn);
	return pawn;
}

Pawn *deserializePawnPointer(Board &board, const uint8_t *buffer, size_t &offset) {
	int id = deserializeInt(buffer, offset);
	if (id == -1)
		return nullptr;
	for (PlayerPawns &group : board.pawnGroups)
		for (Pawn &pawn : group.pawns)
			if (pawn.id == id)
				return &pawn;
	return nullptr;
}

void serialisePlayer(Player player, uint8_t *buffer, size_t &offset) {
	std::memcpy(buffer + offset, &player, sizeof(Player));
	offset += sizeof(Player);
}

void serialisePlayerPointer(Player *player, uint8_t *buffer, size_t &offset) {
	if (player == nullptr){
		serializeInt(-1, buffer, offset);
	} else {
		serializeInt(player->id, buffer, offset);
	}
}

Player deserializePlayer(const uint8_t *buffer, size_t &index) {
	Player player;
	std::memcpy(&player, buffer + index, sizeof(Player));
	index += sizeof(Player);
	return player;
}

Player *deserializePlayerPointer(Board &board, const uint8_t *buffer, size_t &offset) {
	int id = deserializeInt(buffer, offset);
	if (id == -1)
		return nullptr;
	for (Player &player : board.players)
		if (player.id == id)
			return &player;
	return nullptr;
}

void serialisePoint(Point point, uint8_t *buffer, size_t &offset) {
	serializeInt(point.pawnsInside, buffer, offset);
	for (auto &pawn : point.pawns) {
		serialisePawnPointer(pawn, buffer, offset);
	}
}

Point deserializePoint(Board &board, const uint8_t *buffer, size_t &index) {
	Point point;
	point.pawnsInside = deserializeInt(buffer, index);
	for (auto &pawn : point.pawns) {
		pawn = deserializePawnPointer(board, buffer, index);
	}
	return point;
}

void serialiseCourt(Court court, uint8_t *buffer, size_t &offset) {
	serializeInt(court.pawnsInside, buffer, offset);
	for (auto &pawn : court.pawns) {
		serialisePawnPointer(pawn, buffer, offset);
	}
	serializeInt(court.owner->id, buffer, offset);
}

Court deserializeCourt(Board &board, const uint8_t *buffer, size_t &index) {
	Court court;
	court.pawnsInside = deserializeInt(buffer, index);
	for (auto &pawn : court.pawns) {
		pawn = deserializePawnPointer(board, buffer, index);
	}
	court.owner = deserializePlayerPointer(board, buffer, index);
	return court;
}

void serialiseBar(Bar bar, uint8_t *buffer, size_t &offset) {
	serializeInt(bar.pawnsInside, buffer, offset);
	for (auto &pawn : bar.pawns) {
		serialisePawnPointer(pawn, buffer, offset);
	}
}

Bar deserializeBar(Board &board, const uint8_t *buffer, size_t &index) {
	Bar bar;
	bar.pawnsInside = deserializeInt(buffer, index);
	for (auto &pawn : bar.pawns) {
		pawn = deserializePawnPointer(board, buffer, index);
	}
	return bar;
}

void serialisePawnGroup(PlayerPawns group, uint8_t *buffer, size_t &offset) {
	for (auto &pawn : group.pawns) {
		serialisePawn(pawn, buffer, offset);
	}
	serialisePlayerPointer(group.owner, buffer, offset);
}

PlayerPawns deserializePawnGroup(Board &board, const uint8_t *buffer, size_t &offset) {
	PlayerPawns group;
	for (auto &pawn:group.pawns) {
		pawn = deserializePawn(buffer, offset);
	}
	group.owner = deserializePlayerPointer(board, buffer, offset);
	return group;
}

void serialiseBoard(Board &board, uint8_t *buffer, size_t &offset) {
	for (auto &player: board.players) {
		serialisePlayer(player, buffer, offset);
	}
	for (auto &group: board.pawnGroups) {
		serialisePawnGroup(group, buffer, offset);
	}
	serializeInt(board.currentPlayerId, buffer, offset);
	for (auto &point: board.points) {
		serialisePoint(point, buffer, offset);
	}
	for (auto &court: board.courts) {
		serialiseCourt(court, buffer, offset);
	}
	serialiseBar(board.bar, buffer, offset);
	for (auto &dice: board.dices) {
		serializeInt(dice, buffer, offset);
	}
}

Board deserializeBoard(const uint8_t *buffer, size_t &offset) {
	Board board;
	for (auto &player : board.players) {
		player = deserializePlayer(buffer, offset);
	}
	for (auto &group: board.pawnGroups) {
		group = deserializePawnGroup(board, buffer, offset);
	}
	board.currentPlayerId = deserializeInt(buffer, offset);
	for (auto &point:board.points) {
		point = deserializePoint(board, buffer, offset);
	}
	for (auto &court : board.courts) {
		court = deserializeCourt(board, buffer, offset);
	}
	board.bar = deserializeBar(board, buffer, offset);
	for (auto &dice : board.dices) {
		dice = deserializeInt(buffer, offset);
	}
	return board;
}

int main_() {
	Board game = initEverything();
	auto *primeTable = new uint8_t[2000];
	size_t index = 0;

	serialiseBoard(game, primeTable, index);


	index = 0;
	Board newGame = deserializeBoard(primeTable, index);
	cout << newGame.currentPlayerId << " ";
	cout << newGame.pawnGroups[0].pawns[0].id << " ";
	cout << newGame.pawnGroups[0].owner->id << " ";
	cout << newGame.points[0].pawnsInside << " ";
	cout << newGame.courts[0].pawnsInside << " ";
	cout << newGame.bar.pawnsInside << " ";
	cout << newGame.points[0].pawns[0] << " ";
	cout << newGame.bar.pawns[0]->owner->id << " ";

	return 0;
}
