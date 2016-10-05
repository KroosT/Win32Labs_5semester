#include <Windows.h>

#define PARENT_WINDOW_POS 300
#define ID_RB_1 101
#define ID_RB_2 102
#define ID_RB_3 103
#define ID_RB_4 104
#define ID_RB_5 105
#define ID_RB_6 106
#define ID_RB_7 107
#define ID_CB 1001

LPCWSTR g_szClassName = L"myWindowClass";
const int ID_TIMER = 1;
HINSTANCE hInst;
HWND RB_1, RB_2, RB_3, RB_4, RB_5, RB_6, RB_7, CB;
HWND hwnd_child;
int xPos = 0;
int yPos = 0;
int R, G, B;

LRESULT CALLBACK WndProcChild(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void Draw_Star(HDC hdc);
void Draw_Rhombus(HDC hdc);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			//Create child window here.
			WNDCLASS w;
			hInst = hInst;
			memset(&w, 0, sizeof(WNDCLASS));
			w.style = CS_HREDRAW | CS_VREDRAW;
			w.lpfnWndProc = WndProcChild;
			w.hInstance = hInst;
			w.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			w.lpszClassName = L"C Windows";
			w.hCursor = LoadCursor(NULL, IDC_ARROW);
			RegisterClass(&w);
			hwnd_child = CreateWindow(L"C Windows", L"Communicated windows (Lab_005). Child Window.", 
				WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,
				PARENT_WINDOW_POS + 650, PARENT_WINDOW_POS, 600, 480, NULL, NULL, hInst, NULL);
			ShowWindow(hwnd_child, SW_NORMAL);
			UpdateWindow(hwnd_child);
			if (hwnd_child == NULL)
			{
				MessageBox(hwnd, L"Can't create child window.", L"Error creating child window", MB_ICONERROR | MB_OK);
			}

			//Create radiobuttons and checkbox here.
			RB_1 = CreateWindow(L"BUTTON", L"Blue", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 10, 10, 100, 20, hwnd, (HMENU)ID_RB_1, hInst, NULL);
			RB_2 = CreateWindow(L"BUTTON", L"Red", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 10, 35, 100, 20, hwnd, (HMENU)ID_RB_2, hInst, NULL);
			RB_3 = CreateWindow(L"BUTTON", L"Green", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 10, 60, 100, 20, hwnd, (HMENU)ID_RB_3, hInst, NULL);
			RB_4 = CreateWindow(L"BUTTON", L"Star", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 10, 85, 100, 20, hwnd, (HMENU)ID_RB_4, hInst, NULL);
			RB_5 = CreateWindow(L"BUTTON", L"Rectangle", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 10, 110, 100, 20, hwnd, (HMENU)ID_RB_5, hInst, NULL);
			RB_6 = CreateWindow(L"BUTTON", L"Circle", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 10, 135, 100, 20, hwnd, (HMENU)ID_RB_6, hInst, NULL);
			RB_7 = CreateWindow(L"BUTTON", L"Rhombus", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 10, 160, 100, 20, hwnd, (HMENU)ID_RB_7, hInst, NULL);

			CB = CreateWindow(L"BUTTON", L"Check", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 220, 100, 20, hwnd, (HMENU)ID_CB, hInst, NULL);

			//Default choice : Blue
			SendMessage(RB_1, BM_SETCHECK, 1, NULL);
			SendMessage(RB_2, BM_SETCHECK, 0, NULL);
			SendMessage(RB_3, BM_SETCHECK, 0, NULL);

			//Default choice : Star
			SendMessage(RB_4, BM_SETCHECK, 1, NULL);
			SendMessage(RB_5, BM_SETCHECK, 0, NULL);
			SendMessage(RB_6, BM_SETCHECK, 0, NULL);
			SendMessage(RB_7, BM_SETCHECK, 0, NULL);
			break;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case ID_RB_1:
					SendMessage(RB_1, BM_SETCHECK, 1, NULL);
					SendMessage(RB_2, BM_SETCHECK, 0, NULL);
					SendMessage(RB_3, BM_SETCHECK, 0, NULL);
					break;

				case ID_RB_2:
					SendMessage(RB_1, BM_SETCHECK, 0, NULL);
					SendMessage(RB_2, BM_SETCHECK, 1, NULL);
					SendMessage(RB_3, BM_SETCHECK, 0, NULL);
					break;

				case ID_RB_3:
					SendMessage(RB_1, BM_SETCHECK, 0, NULL);
					SendMessage(RB_2, BM_SETCHECK, 0, NULL);
					SendMessage(RB_3, BM_SETCHECK, 1, NULL);
					break;

				case ID_RB_4:
					SendMessage(RB_4, BM_SETCHECK, 1, NULL);
					SendMessage(RB_5, BM_SETCHECK, 0, NULL);
					SendMessage(RB_6, BM_SETCHECK, 0, NULL);
					SendMessage(RB_7, BM_SETCHECK, 0, NULL);
					break;

				case ID_RB_5:
					SendMessage(RB_4, BM_SETCHECK, 0, NULL);
					SendMessage(RB_5, BM_SETCHECK, 1, NULL);
					SendMessage(RB_6, BM_SETCHECK, 0, NULL);
					SendMessage(RB_7, BM_SETCHECK, 0, NULL);
					break;

				case ID_RB_6:
					SendMessage(RB_4, BM_SETCHECK, 0, NULL);
					SendMessage(RB_5, BM_SETCHECK, 0, NULL);
					SendMessage(RB_6, BM_SETCHECK, 1, NULL);
					SendMessage(RB_7, BM_SETCHECK, 0, NULL);
					break;

				case ID_RB_7:
					SendMessage(RB_4, BM_SETCHECK, 0, NULL);
					SendMessage(RB_5, BM_SETCHECK, 0, NULL);
					SendMessage(RB_6, BM_SETCHECK, 0, NULL);
					SendMessage(RB_7, BM_SETCHECK, 1, NULL);
					break;
				
				case ID_CB:
					SendMessage(CB, BM_SETCHECK, 1, NULL);
				
					break;
			}
			break;
		}
		case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			break;
		}
		case WM_DESTROY:
		{
			KillTimer(hwnd, ID_TIMER);
			PostQuitMessage(0);
			break;
		}
		case WM_GETMINMAXINFO:
		{
			MINMAXINFO* mmi = (MINMAXINFO*)lParam;
			mmi->ptMinTrackSize.x = 600;
			mmi->ptMinTrackSize.y = 480;
			mmi->ptMaxTrackSize.x = 600;
			mmi->ptMaxTrackSize.y = 480;
			break;
		}
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		return 0;
}

LRESULT CALLBACK WndProcChild(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
		case WM_LBUTTONDOWN :
		{
			xPos = LOWORD(lParam);
			yPos = HIWORD(lParam);
			InvalidateRect(hwnd_child, 0, 0);
			break;
		}
		case WM_GETMINMAXINFO:
		{
			MINMAXINFO* mmi = (MINMAXINFO*)lParam;
			mmi->ptMinTrackSize.x = 600;
			mmi->ptMinTrackSize.y = 480;
			mmi->ptMaxTrackSize.x = 600;
			mmi->ptMaxTrackSize.y = 480;
			break;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
			
			if (SendMessage(CB, BM_GETCHECK, 1, NULL))
			{
				//Choose color
				if (SendMessage(RB_1, BM_GETCHECK, 1, 0))
				{
					R = 0;
					G = 0;
					B = 255;
				}
				else if (SendMessage(RB_2, BM_GETCHECK, 1, 0))
				{
					R = 255;
					G = 0;
					B = 0;
				}
				else if (SendMessage(RB_3, BM_GETCHECK, 1, 0))
				{
					R = 0;
					G = 255;
					B = 0;
				}

				//Choose figure
				if (SendMessage(RB_4, BM_GETCHECK, 1, NULL))
				{
					Draw_Star(hdc);
				}
				else if (SendMessage(RB_5, BM_GETCHECK, 1, NULL))
				{
					HBRUSH hBrush = CreateSolidBrush(RGB(R, G, B));
					SelectObject(hdc, hBrush);
					Rectangle(hdc, xPos - 30, yPos - 30, xPos + 30, yPos + 30);
				}
				else if (SendMessage(RB_6, BM_GETCHECK, 1, NULL))
				{
					HBRUSH hBrush = CreateSolidBrush(RGB(R, G, B));
					SelectObject(hdc, hBrush);
					Ellipse(hdc, xPos - 30, yPos - 30, xPos + 30, yPos + 30);
				}
				else
				{
					Draw_Rhombus(hdc);
				}
			}
			EndPaint(hwnd, &ps);
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
		L"Communicated windows (Lab_005). Parent Window.",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,
		PARENT_WINDOW_POS, PARENT_WINDOW_POS, 600, 480,
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

void Draw_Rhombus(HDC hdc)
{
	POINT poly[5];

	HBRUSH hBrush = CreateSolidBrush(RGB(R, G, B));
	SelectObject(hdc, hBrush);

	poly[0].x = xPos - 30;
	poly[0].y = yPos;

	poly[1].x = xPos;
	poly[1].y = yPos - 30;

	poly[2].x = xPos + 30;
	poly[2].y = yPos;

	poly[3].x = xPos;
	poly[3].y = yPos + 30;

	poly[4].x = xPos - 30;
	poly[4].y = yPos;

	Polygon(hdc, poly, 5);
}

void Draw_Star(HDC hdc)
{
	POINT poly[10];

	HBRUSH hBrush = CreateSolidBrush(RGB(R, G, B));
	SelectObject(hdc, hBrush);

	poly[0].x = xPos;
	poly[0].y = yPos - 30;

	poly[1].x = xPos + 10;
	poly[1].y = yPos - 10;

	poly[2].x = xPos + 30;
	poly[2].y = yPos - 10;

	poly[3].x = xPos + 10;
	poly[3].y = yPos;

	poly[4].x = xPos + 20;
	poly[4].y = yPos + 20;

	poly[5].x = xPos;
	poly[5].y = yPos + 10;

	poly[6].x = xPos - 20;
	poly[6].y = yPos + 20;

	poly[7].x = xPos - 10;
	poly[7].y = yPos;

	poly[8].x = xPos - 30;
	poly[8].y = yPos - 10;

	poly[9].x = xPos - 10;
	poly[9].y = yPos - 10;

	Polygon(hdc, poly, 10);
}