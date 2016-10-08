#include <Windows.h>

#define ID_EVENT_BUTTON 101
#define ID_CS_BUTTON 102
#define ID_START_BUTTON 103
#define ID_STOP_BUTTON 104

LPCWSTR g_szClassName = L"myWindowClass";
HINSTANCE hInst;
HANDLE threads[3];
HWND hWnd, event_btn, cs_btn;
bool stop_flag = TRUE;
bool thrds_created = FALSE;
bool cs_threads_terminated = FALSE;
bool event_threads_terminated = FALSE;
POINT coords;
POINT start_coords;
CRITICAL_SECTION cs;
HANDLE event;

//if previous_choice FALSE - event, if TRUE - critical section
bool previous_choice = TRUE;

DWORD WINAPI MoveTrain_EventSynchronization(LPVOID lpParam);
DWORD WINAPI MoveTrain_CriticalSection(LPVOID lpParam);
void DrawTrain(HDC hdc);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			event_btn = CreateWindow(L"BUTTON", L"Event", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 10, 70, 60, 30,
				hwnd, (HMENU)ID_EVENT_BUTTON, hInst, NULL);
			cs_btn = CreateWindow(L"BUTTON", L"Critical Section", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 10, 30, 120, 30,
				hwnd, (HMENU)ID_CS_BUTTON, hInst, NULL);

			SendMessage(cs_btn, BM_SETCHECK, 1, NULL);
			SendMessage(event_btn, BM_SETCHECK, 0, NULL);

			//Define start coords of the train.
			coords.x = 5;
			coords.y = 200;
			start_coords = coords;

			//Create event.
			event = CreateEvent(NULL, FALSE, TRUE, NULL);

			break;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case ID_EVENT_BUTTON:
				{
					if (previous_choice) {

						cs_threads_terminated = TRUE;
						event_threads_terminated = FALSE;

						//Wait for threads terminated.
						WaitForMultipleObjects(3, threads, TRUE, INFINITE);
						DeleteCriticalSection(&cs);

						SendMessage(event_btn, BM_SETCHECK, 1, NULL);
						SendMessage(cs_btn, BM_SETCHECK, 0, NULL);

						threads[0] = CreateThread(NULL, NULL, MoveTrain_EventSynchronization, NULL, NULL, NULL);
						threads[1] = CreateThread(NULL, NULL, MoveTrain_EventSynchronization, NULL, NULL, NULL);
						threads[2] = CreateThread(NULL, NULL, MoveTrain_EventSynchronization, NULL, NULL, NULL);

						previous_choice = FALSE;
					}
					break;
				}

				case ID_CS_BUTTON:
				{
					if (!previous_choice) {
						SendMessage(event_btn, BM_SETCHECK, 0, NULL);
						SendMessage(cs_btn, BM_SETCHECK, 1, NULL);

						event_threads_terminated = TRUE;

						WaitForMultipleObjects(3, threads, TRUE, INFINITE);
						cs_threads_terminated = FALSE;

						InitializeCriticalSection(&cs);
									
						threads[0] = CreateThread(NULL, NULL, MoveTrain_CriticalSection, NULL, NULL, NULL);
						threads[1] = CreateThread(NULL, NULL, MoveTrain_CriticalSection, NULL, NULL, NULL);
						threads[2] = CreateThread(NULL, NULL, MoveTrain_CriticalSection, NULL, NULL, NULL);

						previous_choice = TRUE;
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
			HDC hdc = GetDC(hwnd);
			TextOut(hdc, 10, 10, L"Switch synchronization method.", 
				strlen("Switch synchronization method"));
			TextOut(hdc, 10, 111, L"Warning! Avalible only when threads on pause!",
				strlen("Warning! Avalible only when threads on pause!"));
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

DWORD WINAPI MoveTrain_EventSynchronization(LPVOID lpParam) {
	HDC hdc = GetDC(hWnd);
	while (!event_threads_terminated) {
		WaitForSingleObject(event, INFINITE);
		ResetEvent(event);
		coords.x += 10;
		if (coords.x + 80 > 780) {
			RECT rect;
			GetClientRect(hWnd, &rect);
			rect.top = coords.y;
			rect.left = coords.x - 22;
			rect.bottom = coords.y + 70;
			rect.right = coords.x + 62;
			FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
			coords.x = 5;
		}
		DrawTrain(hdc);
		Sleep(1000);
		SetEvent(event);
	}
	return 0;
}

DWORD WINAPI MoveTrain_CriticalSection(LPVOID lpParam) {
	HDC hdc = GetDC(hWnd);
	while (!cs_threads_terminated) {
		EnterCriticalSection(&cs);
		if (!cs_threads_terminated) {
			coords.x += 10;
			if (coords.x + 80 > 780) {
				RECT rect;
				GetClientRect(hWnd, &rect);
				rect.top = coords.y;
				rect.left = coords.x - 22;
				rect.bottom = coords.y + 70;
				rect.right = coords.x + 62;
				FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
				coords.x = 5;
			}
			DrawTrain(hdc);
			Sleep(1000);
		}
		LeaveCriticalSection(&cs);
	}
	return 0;
}

void DrawTrain(HDC hdc)
{
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	HBRUSH hBrush = CreateSolidBrush(RGB(200, 100, 0));
	SelectObject(hdc, hBrush);

	RECT rect;
	GetClientRect(hWnd, &rect);
	rect.top = coords.y;
	rect.left = coords.x - 20;
	rect.bottom = coords.y + 70;
	rect.right = coords.x + 60;
	FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

	POINT train[10];

	train[0].x = coords.x;
	train[0].y = coords.y;

	train[1].x = coords.x + 40;
	train[1].y = coords.y;

	train[2].x = coords.x + 40;
	train[2].y = coords.y + 30;

	train[3].x = coords.x + 60;
	train[3].y = coords.y + 30;

	train[4].x = coords.x + 60;
	train[4].y = coords.y + 10;

	train[5].x = coords.x + 70;
	train[5].y = coords.y + 10;

	train[6].x = coords.x + 70;
	train[6].y = coords.y + 30;

	train[7].x = coords.x + 80;
	train[7].y = coords.y + 30;

	train[8].x = coords.x + 80;
	train[8].y = coords.y + 60;

	train[9].x = coords.x;
	train[9].y = coords.y + 60;

	Polygon(hdc, train, 10);

	hBrush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hdc, hBrush);

	Ellipse(hdc, coords.x + 10, coords.y + 50, coords.x + 30, coords.y + 70);
	Ellipse(hdc, coords.x + 50, coords.y + 50, coords.x + 70, coords.y + 70);
}