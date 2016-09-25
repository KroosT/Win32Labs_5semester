#include <Windows.h>
#include "resource.h"

LPCWSTR g_szClassName = L"myWindowClass";
const int ID_TIMER = 1;

void Flying(RECT* rect);

struct MagicText
{
	int x = 0;
	int y = 0;

	int width;
	int height;

	int dx = 1;
	int dy = 2;

	LPCWSTR text = L"MAGIC TEXT";

	bool is_flying = true;
} magic_text;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			if (!SetTimer(hwnd, ID_TIMER, 30, NULL))
			{
				MessageBox(hwnd, L"Couldn't create a timer.", L"Error!",
					MB_ICONEXCLAMATION | MB_OK);
				return FALSE;
			}
			HDC hdc = GetDC(hwnd);
			SIZE size;

			GetTextExtentPoint32(hdc, magic_text.text, lstrlen(magic_text.text), &size);
			magic_text.width = size.cx;
			magic_text.height = size.cy;
			ReleaseDC(hwnd, hdc);
			break;
		}
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case ID_FILE_START:
				magic_text.is_flying = true;
				break;
			case ID_FILE_STOP:
				magic_text.is_flying = false;
				break;
			}
			break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			KillTimer(hwnd, ID_TIMER);
			PostQuitMessage(0);
			break;
		case WM_TIMER:
		{
			RECT rect;
			HDC hdc = GetDC(hwnd);

			GetClientRect(hwnd, &rect);

			Flying(&rect);
			InvalidateRect(hwnd, NULL, TRUE);
			ReleaseDC(hwnd, hdc);
			break;
		}
		case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT paintStruct;

			hdc = BeginPaint(hwnd, &paintStruct);
			TextOut(hdc, magic_text.x, magic_text.y, magic_text.text,
				lstrlen(magic_text.text));
			EndPaint(hwnd, &paintStruct);
			break;
		}
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

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR1));
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Window Registration Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		L"Flying text (First WinAPI program)",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, L"Window Creation Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

void Flying(RECT* rect)
{
	if (magic_text.is_flying)
	{
		magic_text.x += magic_text.dx;
		magic_text.y += magic_text.dy;

		if (magic_text.x < 0)
		{
			magic_text.x = 0;
			magic_text.dx *= -1;
		}
		else if (magic_text.x + magic_text.width > rect->right)
		{
			magic_text.x = rect->right - magic_text.width;
			magic_text.dx *= -1;
		}

		if (magic_text.y < 0)
		{
			magic_text.y = 0;
			magic_text.dy *= -1;
		}
		else if (magic_text.y + magic_text.height > rect->bottom)
		{
			magic_text.y = rect->bottom - magic_text.height;
			magic_text.dy *= -1;
		}
	}
}