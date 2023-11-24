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
		player = Player{0, "Hello", false, 4};
	}
	for (int i = 0; i < PAWNS_PER_PLAYER; ++i) {
		board.pawns[i] = Pawn{0, i, false, false, PAWN_WHITE, -1};
		board.pawns[PAWNS_PER_PLAYER + i] = Pawn{1, PAWNS_PER_PLAYER + i, false, false, PAWN_BLACK, 1};
	}
	for (auto &point: board.points) {
		point = Point{0};
		for (int &j: point.pawnsId) {
			j = -1;
		}
	}
	for (auto &court: board.courts) {
		court = Court{.pawnsInside=0, .ownerId=2};
		for (int &j: court.pawnsId) {
			j = -1;
		}
	}
	board.bar = Bar{0};
	for (int &i: board.bar.pawnsId) {
		i = -1;
	}
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

Pawn deserializePawn(const uint8_t *buffer, size_t &index) {
	Pawn pawn;
	std::memcpy(&pawn, buffer + index, sizeof(Pawn));
	index += sizeof(Pawn);
	return pawn;
}

void serialisePlayer(Player player, uint8_t *buffer, size_t &offset) {
	std::memcpy(buffer + offset, &player, sizeof(Player));
	offset += sizeof(Player);
}

Player deserializePlayer(const uint8_t *buffer, size_t &index) {
	Player player;
	std::memcpy(&player, buffer + index, sizeof(Player));
	index += sizeof(Player);
	return player;
}

void serialisePoint(Point point, uint8_t *buffer, size_t &offset) {
	std::memcpy(buffer + offset, &point, sizeof(Point));
	offset += sizeof(Point);
}

Point deserializePoint(const uint8_t *buffer, size_t &index) {
	Point value;
	std::memcpy(&value, buffer + index, sizeof(Point));
	index += sizeof(Point);
	return value;
}

void serialiseCourt(Court court, uint8_t *buffer, size_t &offset) {
	std::memcpy(buffer + offset, &court, sizeof(Court));
	offset += sizeof(Court);
}

Court deserializeCourt(const uint8_t *buffer, size_t &index) {
	Court court;
	std::memcpy(&court, buffer + index, sizeof(Court));
	index += sizeof(Court);
	return court;
}

void serialiseBar(Bar bar, uint8_t *buffer, size_t &offset) {
	std::memcpy(buffer + offset, &bar, sizeof(Bar));
	offset += sizeof(Bar);
}

Bar deserializeBar(const uint8_t *buffer, size_t &index) {
	Bar bar;
	std::memcpy(&bar, buffer + index, sizeof(Bar));
	index += sizeof(Bar);
	return bar;
}

void serialiseBoard(Board &board, uint8_t *buffer, size_t &offset) {
	for (auto &pawn: board.pawns) {
		serialisePawn(pawn, buffer, offset);
	}
	for (auto &player: board.players) {
		serialisePlayer(player, buffer, offset);
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

Board deserialiseBoard(const uint8_t *buffer, size_t &offset) {
	Board board;
	for (auto &pawn: board.pawns) {
		pawn = deserializePawn(buffer, offset);
	}
	for (auto &player : board.players) {
		player = deserializePlayer(buffer, offset);
	}
	board.currentPlayerId = deserializeInt(buffer, offset);
	for (auto &point:board.points) {
		point = deserializePoint(buffer, offset);
	}
	for (auto &court : board.courts) {
		court = deserializeCourt(buffer, offset);
	}
	board.bar = deserializeBar(buffer, offset);
	for (auto &dice : board.dices) {
		dice = deserializeInt(buffer, offset);
	}
	return board;
}

int main() {
	Board game = initEverything();
	auto *primeTable = new uint8_t[sizeof(Board)];
	size_t index = 0;

	serialiseBoard(game, primeTable, index);

	index = 0;
	Board newGame = deserialiseBoard(primeTable, index);
	cout << newGame.currentPlayerId << " " << newGame.pawns[0].id << " " << newGame.points[0].pawnsId[2] << " " << newGame.players[0].isHisTurn;

	return 0;
}
