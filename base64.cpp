//
// Created by Filip Dabkowski on 24/11/2023.
//

#include "base64.h"
#include "cstring"
#include "cstdint"

#include "iostream"


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

int finalEncodedDataLen(int len) {
	int count = len / 3 * 4;
	if (len % 3)
		count += 4;
	return count;
}

int toNumber(const uint8_t *byteBlock, int blockSize = 3) {
	int res = 0;
	for (int i = 0; i < blockSize; ++i) {
		res <<= 8;
		res += byteBlock[i];
	}
	for (int _ = 0; _ < 3 - blockSize; _++) {
		res <<= 8;
	}
	return res;
}

void toBase64(int block, char *finalData, int &offset, int padding) {
	for (int i = 4 - 1; i >= 0; --i) {
		int mask = createBitMask(6, i * 6);
		int encoded = (block & mask) >> i * 6;
		if (i < padding) {
			finalData[offset++] = '=';
		} else {
			finalData[offset++] = base64table[encoded];
		}
	}
}

char *encodeBase64(uint8_t *data, size_t length) {
	int finalLen = finalEncodedDataLen((int) (length));
	char *finalData = new char[finalLen];

	int encodedChar = 0;
	for (int i = 0; i < length; i += 3) {
		int padding = 0;
		if (i + 3 > length) {
			padding = (i + 3) - (int) (length);
		}
		int block = toNumber(data + i, 3 - padding);
		toBase64(block, finalData, encodedChar, padding);
	}

	return finalData;
}

//uint8_t * decodeBase64(const char *base64String, int length) {
//	int paddingCount = 0;
//	int encodedNumber = 0;
//	for (int i = 0; i < 4; ++i) {
//		char tempChar = base64String[i];
//		if (tempChar == '=') {
//			encodedNumber <<= 6;
//			paddingCount++;
//		} else {
//			encodedNumber <<= 6;
//			encodedNumber += indexOf(base64table, tempChar, baseSize);
//		}
//
//		if (i == 3) {
//			vector<char> decodedBytes;
//			for (int j = 0; j < 3 - paddingCount; ++j) {
//				int mask = createBitMask(8, 8 * (2 - j));
//				int decodedNumber = (encodedNumber & mask) >> 8 * (2 - j);
//
//				decodedBytes.push_back((char) (decodedNumber));
//			}
//			return decodedBytes;
//		}
//	}
//	return {};
//}

int main() {
	const int len = 4;
	uint8_t test[len] = {'h', 'e', 'l', 'l'};
	char *res = encodeBase64(test, len);
	std::cout << res;
	delete[] res;
	return 0;
}
