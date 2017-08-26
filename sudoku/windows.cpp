#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance,
	PSTR szCmdLine, int iCmdShow) {
	
	static TCHAR szAppName[] = TEXT("sudoku");
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

	int cx = GetSystemMetrics(SM_CXSCREEN)/2;
	int cy = GetSystemMetrics(SM_CYSCREEN)/2;

	hwnd = CreateWindow(szAppName,	//szWindowClass:		the name of the application
		TEXT("sudoku"),				//szTitle:				the text that appears in the title bar
		WS_OVERLAPPEDWINDOW,		//WS_OVERLAPPEDWINDOW:	the type of window to create
		cx,							//CW_USEDEFAULT:		initial position x
		cy,							//CW_USEDEFAULT:		initial position y
		500,						//						initial size width
		500,						//						initial size height
		NULL,						//						the parent of this window
		NULL,						//						this application does not have a menu bar
		hInstance,					//hInstance:			the first parameter from WinMain
		NULL);						//						not used in this application

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
		return 0;
	case WM_PAINT:

		HDC hdc;
		PAINTSTRUCT ps;
		RECT rect;

		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);

		DrawText(hdc, TEXT("hello"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}