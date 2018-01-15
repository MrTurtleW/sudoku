#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include "resource.h"
#include "GamePad.h"

static TCHAR szAppName[] = TEXT("sudoku");
static TCHAR szAppTitle[] = TEXT("sudoku");
static TCHAR szChildClass[] = TEXT("cell");

static HWND hwndStatus;
static HWND* hwndChild;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);

GamePad gamePad = GamePad();

HWND CreateStatusBar(HWND hwnd) {
	HINSTANCE hInst = GetModuleHandle(NULL);
	HWND hwndStatus = CreateWindow(STATUSCLASSNAME, TEXT(""),
		WS_CHILD | WS_VISIBLE,
		0, 0, 0, 0,
		hwnd,
		NULL,
		hInst,
		NULL);

	if (hwndStatus) {
		int array[3] = { gamePad.singleCellLength, gamePad.singleCellLength*3, -1};
		SendMessage(hwndStatus, SB_SETPARTS, (WPARAM)3, (LPARAM)array);
		SendMessage(hwndStatus, SB_SETTEXT, (LPARAM)0, (WPARAM)TEXT("Ready"));
	}

	return hwndStatus;
}

HWND* CreateChildWindow(HWND hwnd) {
	HWND childHwnd[9][9];
	HINSTANCE hInst = GetModuleHandle(NULL);
	for (int x = 0; x < 9; x++) {
		for (int y = 0; y < 9; y++) {
			childHwnd[x][y] = CreateWindow(szChildClass,
				NULL,
				WS_CHILDWINDOW | WS_VISIBLE,
				x*gamePad.singleCellLength,
				y*gamePad.singleCellLength,
				gamePad.singleCellLength,
				gamePad.singleCellLength,
				hwnd,
				NULL,
				hInst,
				NULL);
		}
	}

	return *childHwnd;
}

void UpdateCellPos(HWND hwnd, LPARAM lParam) {
	int cellX = HIWORD(lParam) / gamePad.singleCellLength + 1;
	int cellY = LOWORD(lParam) / gamePad.singleCellLength + 1;
	if (cellX < 10 && cellY < 10) {
		TCHAR szBuf[MAX_PATH];
		_stprintf_s(szBuf, TEXT("cell (%d,%d)"), cellX, cellY);
		SendMessage(hwnd, SB_SETTEXT, 1, (LPARAM)(LPSTR)szBuf);
	}
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance,
	PSTR szCmdLine, int iCmdShow) {

	HWND hwnd;
	MSG msg;
	WNDCLASS wndClass;

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = (TCHAR *)IDR_MENU;
	wndClass.lpszClassName = szAppName;

	if (!RegisterClass(&wndClass)) {
		MessageBox(NULL, 
			TEXT("Regist window class error, exiting!"), szAppName, MB_ICONERROR);

		return 0;
	}

	wndClass.lpfnWndProc = ChildWndProc;
	wndClass.cbWndExtra = sizeof(long);
	wndClass.hIcon = NULL;
	wndClass.lpszClassName = szChildClass;

	RegisterClass(&wndClass);

	hwnd = CreateWindow(szAppName,				//szWindowClass:		the name of the application
		szAppTitle,								//szTitle:				the text that appears in the title bar
		WS_OVERLAPPEDWINDOW^WS_THICKFRAME,		//WS_OVERLAPPEDWINDOW:	the type of window to create
		gamePad.startX,							//CW_USEDEFAULT:		initial position x
		gamePad.startY,							//CW_USEDEFAULT:		initial position y
		gamePad.width,							//						initial size width
		gamePad.height,							//						initial size height
		NULL,									//						the parent of this window
		NULL,									//						this application does not have a menu bar
		hInstance,								//hInstance:			the first parameter from WinMain
		NULL);									//						not used in this application

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {
	case WM_COMMAND:

		HMENU hMenu;

		hMenu = GetMenu(hwnd);
		switch (LOWORD(wParam)) {
		case ID_FILE_NEWGAME:
			return 0;
		case ID_FILE_OPEN:
			return 0;
		case ID_FILE_SAVE:
			return 0;
		case ID_FILE_EXIT:
			PostQuitMessage(0);
			return 0;
		case ID_EDIT_UNDO:
			return 0;
		case ID_EDIT_REDO:
			return 0;
		case ID_EDIT_RESET:
			return 0;
		}
		break;
	case WM_CREATE:
		hwndStatus = CreateStatusBar(hwnd);
		hwndChild = CreateChildWindow(hwnd);
		return 0;
	case WM_PAINT:
		
		return 0;
	case WM_MOUSEMOVE:
		UpdateCellPos(hwndStatus, lParam);
		return 0;
	case WM_SIZE:
		
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC         hdc;
	PAINTSTRUCT ps;
	RECT        rect;

	switch (message)
	{
	case WM_CREATE:
		return 0;

	case WM_MOUSEMOVE:
		UpdateCellPos(hwndStatus, lParam);
		return 0;

	case WM_LBUTTONDOWN:
		SetWindowLong(hwnd, 0, 1 ^ GetWindowLong(hwnd, 0));
		SetFocus(hwnd);
		InvalidateRect(hwnd, NULL, FALSE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		GetClientRect(hwnd, &rect);
		Rectangle(hdc, 0, 0, rect.right, rect.bottom);

		if (GetWindowLong(hwnd, 0))
		{
			MoveToEx(hdc, 0, 0, NULL);
			LineTo(hdc, rect.right, rect.bottom);
			MoveToEx(hdc, 0, rect.bottom, NULL);
			LineTo(hdc, rect.right, 0);
		}

		if (hwnd == GetFocus())
		{
			rect.left += rect.right / 10;
			rect.right -= rect.left;
			rect.top += rect.bottom / 10;
			rect.bottom -= rect.top;

			SelectObject(hdc, GetStockObject(NULL_BRUSH));
			SelectObject(hdc, CreatePen(PS_DASH, 0, 0));
			Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
			DeleteObject(SelectObject(hdc, GetStockObject(BLACK_PEN)));
		}

		EndPaint(hwnd, &ps);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}