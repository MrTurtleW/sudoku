#pragma once

#include <windows.h>

class GamePad {

public:
	GamePad();
	void init(HWND hwnd);

	int singleCellLength;
	int thinLineWidth;
	int thickLineWidth;
	int width;
	int height;

	int startX;
	int startY;

private:
	int originalData[9][9];
	int currentData[9][9];
};