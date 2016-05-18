#include "stdafx.h"
#pragma once

// Splits given integer in bytes
inline void ByteShift(int num, int numFrames, int8_t** frames, bool lowToHigh = false) {
	*frames = new int8_t[numFrames];
	for (int i = 0; i < numFrames; i++) {
		(*frames)[i] = (num >> 8 * (lowToHigh ? i : numFrames - i - 1)) & 0xFF;
	}
}

// Recombines different bytes into an int
inline int ByteUnshift(const int8_t* frames, int numFrames, bool lowToHigh = false) {
	int result = 0;
	for (int i = 0; i < numFrames; i++)
		result += (frames[i] << 8 * (lowToHigh ? i : numFrames - i - 1));
	return result;
}

// Copies an array deeply
inline void DeepCopy(const int8_t* const source, int size, int8_t* const dest) {
	//dest = new int8_t[size];
	for (int i = 0; i < size; i++)
		dest[i] = source[i];
}