#include "stdafx.h"
#pragma once


inline void ByteShift(int num, int numFrames, int8_t** frames, bool lowToHigh = false) {
	*frames = new int8_t[numFrames];
	for (int i = 0; i < numFrames; i++) {
		(*frames)[i] = (num >> 8 * (lowToHigh ? i : numFrames - i - 1)) & 0xFF;
	}
}

inline int ByteUnshift(const int8_t* frames, int numFrames, bool lowToHigh = false) {
	int result = 0;
	for (int i = 0; i < numFrames; i++)
		result += (frames[i] << 8 * (lowToHigh ? i : numFrames - i - 1));
	return result;
}