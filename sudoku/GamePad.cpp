#include "GamePad.h"

GamePad::GamePad() {

	singleCellLength = 60;


	width = singleCellLength * 9 + 16;
	height = singleCellLength * 10 + 19;

	startX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	startY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
}

void GamePad::init(HWND hwnd) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	EndPaint(hwnd, &ps);
}