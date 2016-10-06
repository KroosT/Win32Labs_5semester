#include <Windows.h>

#define ID_START_BUTTON 101
#define ID_STOP_BUTTON 102
#define ID_LEFT_THREAD 1001
#define ID_RIGHT_THREAD 1002
#define ID_CENTER_THREAD 1003

LPCWSTR g_szClassName = L"myWindowClass";
HINSTANCE hInst;
HWND hWnd, Start_Button, Stop_Button;
HANDLE threads[3];
bool threads_exist = FALSE;
bool stop_flag = TRUE;
POINT left_line_coords;
POINT right_line_coords;
POINT center_line_coords;

DWORD WINAPI DrawLeftLine(LPVOID lpParam);
DWORD WINAPI DrawRightLine(LPVOID lpParam);
DWORD WINAPI DrawCenterLine(LPVOID lpParam);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		Start_Button = CreateWindow(L"Button", L"Start", WS_CHILD | WS_VISIBLE | WS_BORDER, 200, 500, 100, 30, hwnd,
			(HMENU)ID_START_BUTTON, hInst, NULL);
		Stop_Button = CreateWindow(L"Button", L"Stop", WS_CHILD | WS_VISIBLE | WS_BORDER, 500, 500, 100, 30, hwnd,
			(HMENU)ID_STOP_BUTTON, hInst, NULL);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			case ID_START_BUTTON:
				if (stop_flag) {
					if (threads_exist)
					{
						ResumeThread(threads[0]);
						ResumeThread(threads[1]);
						ResumeThread(threads[2]);
					}
					else
					{
						left_line_coords.x = 100;
						left_line_coords.y = 0;

						right_line_coords.x = 700;
						right_line_coords.y = 0;

						center_line_coords.x = 400;
						center_line_coords.y = 0;

						threads[0] = CreateThread(NULL, NULL, DrawLeftLine, &left_line_coords, NULL, NULL);
						threads[1] = CreateThread(NULL, NULL, DrawRightLine, &right_line_coords, NULL, NULL);
						threads[2] = CreateThread(NULL, NULL, DrawCenterLine, &center_line_coords, NULL, NULL);

						threads_exist = TRUE;
					}
					stop_flag = FALSE;
				}
				break;

			case ID_STOP_BUTTON:
				if (!stop_flag)
				{
					SuspendThread(threads[0]);
					SuspendThread(threads[1]);
					SuspendThread(threads[2]);
					stop_flag = TRUE;
				}
				break;
		}
		break;
	}
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* mmi = (MINMAXINFO*)lParam;
		mmi->ptMinTrackSize.x = 800;
		mmi->ptMinTrackSize.y = 600;
		mmi->ptMaxTrackSize.x = 800;
		mmi->ptMaxTrackSize.y = 600;
		break;
	}
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	hInst = hInstance;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Window Registration Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		L"Threads (Lab_006)",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, L"Window Creation Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hWnd = hwnd;

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

//draw left line
DWORD WINAPI DrawLeftLine(LPVOID lpParam)
{
	PAINTSTRUCT ps;
	HPEN hPenLeft;

	//define right clearing rectangle
	RECT rect;
	GetClientRect(hWnd, &rect);
	rect.bottom = rect.bottom - 100;
	rect.right = rect.right - 600;

	POINT coords = *((POINT*) lpParam);

	HDC hdc = GetDC(hWnd);

	hPenLeft = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
	SelectObject(hdc, hPenLeft);
	MoveToEx(hdc, coords.x, coords.y, NULL);
	while (TRUE) 
	{
		coords.y += 10;
		if (coords.y > 450) {
			coords.y = 0;
			MoveToEx(hdc, coords.x, coords.y, NULL);
			FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
		}
		else {
			LineTo(hdc, coords.x, coords.y);
		}
		Sleep(600);
	}
	DeleteObject(hPenLeft);
	return 0;
}

//draw right line
DWORD WINAPI DrawRightLine(LPVOID lpParam)
{
	PAINTSTRUCT ps;
	HPEN hPenRight;

	//define right clearing rectangle
	RECT rect;
	GetClientRect(hWnd, &rect);
	rect.bottom = rect.bottom - 100;
	rect.left = rect.left + 500;

	POINT coords = *((POINT*)lpParam);

	HDC hdc = GetDC(hWnd);

	hPenRight = CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
	SelectObject(hdc, hPenRight);
	MoveToEx(hdc, coords.x, coords.y, NULL);
	while (TRUE) {
		coords.y += 10;
		if (coords.y > 450) {
			coords.y = 0;
			MoveToEx(hdc, coords.x, coords.y, NULL);
			FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
		}
		else {
			LineTo(hdc, coords.x, coords.y);
		}
		Sleep(500);
		DeleteObject(hPenRight);
	}
	return 0;
}

//draw center line
DWORD WINAPI DrawCenterLine(LPVOID lpParam)
{
	PAINTSTRUCT ps;
	HPEN hPenCenter;

	//define center clearing rectangle
	RECT rect;
	GetClientRect(hWnd, &rect);
	rect.bottom = rect.bottom - 100;
	rect.left = rect.left + 300;
	rect.right = rect.right - 300;

	POINT coords = *((POINT*)lpParam);

	HDC hdc = GetDC(hWnd);

	hPenCenter = CreatePen(PS_SOLID, 5, RGB(0, 0, 255));
	SelectObject(hdc, hPenCenter);
	MoveToEx(hdc, coords.x, coords.y, NULL);
	while (TRUE) {
		coords.y += 10;
		if (coords.y > 450) {
			coords.y = 0;
			MoveToEx(hdc, coords.x, coords.y, NULL);
			FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
		}
		else {
			LineTo(hdc, coords.x, coords.y);
		}
		Sleep(400);
	}
	DeleteObject(hPenCenter);
	return 0;
}