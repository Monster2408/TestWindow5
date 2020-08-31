#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <stdio.h>

#define _WIN32_WINNT 0x0500
#define WINDOW_WIDTH    1280     // ソフト起動後に表示する初期ウインドウ横サイズ
#define WINDOW_HEIGHT   720      // ソフト起動後に表示する初期ウインドウ縦サイズ

#define BUTTON_ID1 0             // Start Button
#define BUTTON_ID2 1             // End Button
#define BUTTON_IDA 2             // A Button
#define BUTTON_IDB 3             // B Button

#define TITLE _T("TEST")

HWND MainHwnd;
HWND Button_1, Button_2, Button_A, Button_B; // ボタン用

RECT recDisplay, recWindow, recClient;

BOOLEAN DebugMode = TRUE;

void DoGetActiveWindow(); // DoGetActiveWindow() の宣言

int main() // プログラム実行後に実行される関数？
{
	if (!DebugMode) {
		HWND hWnd = GetConsoleWindow(); // コンソールウインドウ取得
		ShowWindow(hWnd, SW_MINIMIZE);  // コンソールウインドウ最小化？
		ShowWindow(hWnd, SW_HIDE);      // コンソールウインドウ隠す
	}
	DoGetActiveWindow();            // DoGetActiveWindow() の実行
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	HDC hdc;
	RECT rec;
	RECT rect;
	PAINTSTRUCT ps;
	int width = WINDOW_WIDTH;
	int height = WINDOW_HEIGHT;

	static HWND hPush;

	switch (msg) {  // 投げられたメッセージを発見(?)

	case WM_CREATE:
		createButtons(hwnd, wp, lp, width);
		return 0;
		/*
			ウインドウが閉じられたら動作終了
		*/
	case WM_DESTROY:                                                                      // ウインドウを閉じた場合
		PostQuitMessage(0);                                                               // システムを終わらす
		return 0;
		/*
			コマンド
		*/
	case WM_COMMAND:                                                                      // コマンドであった場合
		switch (LOWORD(wp)) {
			/*
				画面サイズの変更に応じてボタンの位置を変更する
				完成しなかった
			*/
		case WM_SIZE:
			updateButtons(hwnd, wp, lp);
			return 0;
			/*
			画面が完成したらボタンを配置する
			*/
			/*
				ボタンのIDから押したボタンを取得
			*/
		case BUTTON_ID1:                                                                  // ボタン１であった場合
			MessageBox(hwnd, TEXT("BUTTON_ID1"), TEXT("Kitty"), MB_OK);                   // OK
			break;
		case BUTTON_ID2:                                                                  // ボタン２であった場合
			DestroyWindow(hwnd);                                                          // システムを停止
			break;
		case BUTTON_IDA:                                                                  // ボタンAであった場合
			MessageBox(hwnd, TEXT("BUTTON_IDA"), TEXT("Kitty"), MB_CANCELTRYCONTINUE);    // キャンセル, 続行 or OK
			break;
		case BUTTON_IDB:                                                                  // ボタンであった場合
			MessageBox(hwnd, TEXT("BUTTON_ID1"), TEXT("Kitty"), MB_HELP);                 // ヘルプ
			break;
		}
		return 0;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

int updateButtons(HWND hwnd, WPARAM wp, LPARAM lp) {
	RECT rect;
	int width = WINDOW_WIDTH;

	if (GetWindowRect(MainHwnd, &rect))
	{
		width = rect.right - rect.left;
	}
	width = (width / 2) - 200;

	SetWindowPos(Button_1, NULL, width, 420, 400, 50, (SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE));
	MoveWindow(Button_A, width, 360, 195, 50, TRUE);
	MoveWindow(Button_B, width + 205, 360, 195, 50, TRUE);
	MoveWindow(Button_2, width, 420, 400, 50, TRUE);

	UpdateWindow(Button_1, NULL, width, 420, 400, 50);
	return 0;
}

int createButtons(HWND hwnd, WPARAM wp, LPARAM lp, int width) {
	RECT rect;
	if (GetWindowRect(hwnd, &rect))
	{
		width = rect.right - rect.left;
	}
	width = (width / 2) - 200;
	Button_1 = CreateWindow(
		TEXT("BUTTON"),
		TEXT("START"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		width,
		300, // 基準の高さ(Y座標は前の座標に+60)
		400,
		50,
		hwnd,
		(HMENU)BUTTON_ID1,
		((LPCREATESTRUCT)(lp))->hInstance,
		NULL
	);

	Button_A = CreateWindow(
		TEXT("BUTTON"),
		TEXT("設定"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		width,
		360, // 基準の高さ(Y座標は前の座標に+60)
		195,
		50,
		hwnd,
		(HMENU)BUTTON_IDA,
		((LPCREATESTRUCT)(lp))->hInstance,
		NULL
	);

	Button_B = CreateWindow(
		TEXT("BUTTON"),
		TEXT("ヘルプ"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		width + 205,
		360, // 基準の高さ(Y座標は前の座標に+60)
		195,
		50,
		hwnd,
		(HMENU)BUTTON_IDB,
		((LPCREATESTRUCT)(lp))->hInstance,
		NULL
	);

	Button_2 = CreateWindow(
		TEXT("BUTTON"),
		TEXT("CLOSE"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		width,
		420,
		400,
		50,
		hwnd,
		(HMENU)BUTTON_ID2,
		((LPCREATESTRUCT)(lp))->hInstance,
		NULL
	);
	return 0;
}

/* アクティブウィンドウを表示する */
void DoGetActiveWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	HWND hWnd, hDeskWnd;
	WNDCLASS winc;
	MSG msg;

	hDeskWnd = GetDesktopWindow();                              // PCの画面サイズ取得
	GetWindowRect(hDeskWnd, &recDisplay);                       // 画面に関するデータ取得

	winc.style = CS_HREDRAW | CS_VREDRAW;                       // いろいろ...
	winc.lpfnWndProc = WindowProc;
	winc.cbClsExtra = winc.cbWndExtra = 0;
	winc.hInstance = hInstance;
	winc.hIcon = LoadIcon(NULL, NULL);
	winc.hCursor = LoadCursor(NULL, IDC_ARROW);
	winc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszMenuName = NULL;
	winc.lpszClassName = TEXT("MainWindow");

	if (!RegisterClass(&winc)) return 0;                        // ウインドウが表示できなかったら終了

	/* ウインドウの詳細設定 */
	MainHwnd = CreateWindow(
		TEXT("MainWindow"),
		TEXT("検定対策ソフト"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		(recDisplay.right - WINDOW_WIDTH) / 2,                 //左上x座標
		(recDisplay.bottom - WINDOW_HEIGHT) / 2,               //左上y座標
		WINDOW_WIDTH, //幅
		WINDOW_HEIGHT, //高さ
		NULL, NULL,
		hInstance, NULL
	);

	/* 文字を設定 */
	HDC hdc;
	RECT rect;
	hdc = GetDC(MainHwnd);
	LPTSTR lptStr = _T("検定対策用ソフト");
	TextOut(hdc, 10, 10, lptStr, lstrlen(lptStr));
	ReleaseDC(MainHwnd, hdc);

	/* ウインドウを最前面に */
	SetWindowPos(
		MainHwnd,
		HWND_TOPMOST,
		(recDisplay.right - WINDOW_WIDTH) / 2, //左上x座標
		(recDisplay.bottom - WINDOW_HEIGHT) / 2, //左上y座標
		WINDOW_WIDTH, //幅
		WINDOW_HEIGHT, //高さ
		NULL
	);

	UpdateWindow(MainHwnd);
	ShowWindow(MainHwnd, SW_SHOW);

	if (MainHwnd == NULL) return 0;

	while (GetMessage(&msg, NULL, 0, 0)) DispatchMessage(&msg);
	return msg.wParam;
}
