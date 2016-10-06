#include <Windows.h>

#define ID_EVENT_BUTTON 101
#define ID_CS_BUTTON 102
#define ID_START_BUTTON 103
#define ID_STOP_BUTTON 104

LPCWSTR g_szClassName = L"myWindowClass";
HINSTANCE hInst;
HANDLE threads[3];
HWND event_btn, cs_btn, start_btn, stop_btn;
bool stop_flag = TRUE;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			event_btn = CreateWindow(L"BUTTON", L"Event", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 10, 10, 60, 30,
				hwnd, (HMENU)ID_EVENT_BUTTON, hInst, NULL);
			cs_btn = CreateWindow(L"BUTTON", L"Critical Section", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 10, 50, 120, 30,
				hwnd, (HMENU)ID_CS_BUTTON, hInst, NULL);
			cs_btn = CreateWindow(L"BUTTON", L"Start", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 500, 100, 30,
				hwnd, (HMENU)ID_START_BUTTON, hInst, NULL);
			cs_btn = CreateWindow(L"BUTTON", L"Stop", WS_VISIBLE | WS_CHILD | WS_BORDER, 500, 500, 100, 30,
				hwnd, (HMENU)ID_STOP_BUTTON, hInst, NULL);

			SendMessage(event_btn, BM_SETCHECK, 1, NULL);
			SendMessage(cs_btn, BM_SETCHECK, 0, NULL);
			break;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case ID_EVENT_BUTTON:
				{
					SendMessage(event_btn, BM_SETCHECK, 1, NULL);
					SendMessage(cs_btn, BM_SETCHECK, 0, NULL);
					break;
				}

				case ID_CS_BUTTON:
				{
					SendMessage(event_btn, BM_SETCHECK, 0, NULL);
					SendMessage(cs_btn, BM_SETCHECK, 1, NULL);
					break;
				}

				case ID_START_BUTTON:
				{
					if (stop_flag)
					{

						stop_flag = FALSE;
					}
					break;
				}

				case ID_STOP_BUTTON:
				{
					if (!stop_flag)
					{

						stop_flag = TRUE;
					}
					break;
				}
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
		case WM_PAINT:
		{

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
		L"Synchronization (Lab_007)",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,
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