//
// Created by Filip Dabkowski on 24/11/2023.
//

#include "base64.h"

#include "iostream"
#include "vector"

using namespace std;

const int baseSize = 64;
const char base64table[baseSize] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
									'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
									'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
									'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

int createBitMask(int numberOfUpBits, int rightOffset) {
	// Other option: ((uint) (-1)) << (32-upBits) >> (32-upBits-offset)
	return ((1 << numberOfUpBits) - 1) << rightOffset;
}

int indexOf(const char *searchTable, char searchElement, int maxElement) {
	for (int i = 0; i < maxElement; ++i)
		if (searchTable[i] == searchElement)
			return i;
	return -1;
}

vector<char> decodeBase64(const char base64String[]) {
	int paddingCount = 0;
	int encodedNumber = 0;
	for (int i = 0; i < 4; ++i) {
		char tempChar = base64String[i];
		if (tempChar == '=') {
			encodedNumber <<= 6;
			paddingCount++;
		} else {
			encodedNumber <<= 6;
			encodedNumber += indexOf(base64table, tempChar, baseSize);
		}

		if (i == 3) {
			vector<char> decodedBytes;
			for (int j = 0; j < 3 - paddingCount; ++j) {
				int mask = createBitMask(8, 8 * (2 - j));
				int decodedNumber = (encodedNumber & mask) >> 8 * (2 - j);

				decodedBytes.push_back((char) (decodedNumber));
			}
			return decodedBytes;
		}
	}
	return {};
}
