#include <Windows.h>
#include <conio.h>

#define ID_BUTTON_DRAW 101
#define ID_BUTTON_CLEAR 102

LPCWSTR g_szClassName = L"myWindowClass";
void CreateButtons(HWND hwnd);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

	case WM_CREATE:
	{
		CreateButtons(hwnd);

		break;
	}

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;
		FillRect(pdis->hDC, &pdis->rcItem, (HBRUSH)CreateSolidBrush(RGB(150, 50, 200)));
		FrameRect(pdis->hDC, &pdis->rcItem, (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)));

		SetBkMode(pdis->hDC, TRANSPARENT);
		SetTextColor(pdis->hDC, RGB(0, 255, 100));

		if (pdis->CtlID == ID_BUTTON_DRAW)
		{
			DrawText(pdis->hDC, L"Draw", -1, &pdis->rcItem, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}
		else if (pdis->CtlID == ID_BUTTON_CLEAR)
		{
			DrawText(pdis->hDC, L"Clear", -1, &pdis->rcItem, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}
		switch (pdis->CtlID)
		{
			case ID_BUTTON_DRAW:
			{
				Ellipse(pdis->hDC, 10, 15, 30, 35);
				Ellipse(pdis->hDC, 13, 18, 18, 23);
				Ellipse(pdis->hDC, 20, 18, 25, 23);
				HPEN hPen1 = CreatePen(PS_SOLID, 2, RGB(102, 0, 255));
				MoveToEx(pdis->hDC, 15, 27, NULL);
				LineTo(pdis->hDC, 20, 30);
				LineTo(pdis->hDC, 25, 27);
				break;
			}
			case ID_BUTTON_CLEAR:
			{
				Rectangle(pdis->hDC, 5, 15, 25, 35);
				HPEN hPen1 = CreatePen(PS_SOLID, 2, RGB(102, 0, 255));
				MoveToEx(pdis->hDC, 5, 15, NULL);
				LineTo(pdis->hDC, 25, 35);
				MoveToEx(pdis->hDC, 25, 15, NULL);
				LineTo(pdis->hDC, 5, 35);
				break;
			}
		}
		break;
	}

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			case ID_BUTTON_DRAW:
			{
				HDC hdc = GetDC(hwnd); //получаем DC(контекст устройства) дл€ рисовани€
				HPEN hPen1; //объ€вл€ем объект перо
				HGDIOBJ hPenOld, hBrushOld;
				HBRUSH hBrush; //объ€вл€ем кисть

				hPen1 = CreatePen(PS_SOLID, 2, RGB(102, 0, 255)); //логическое перо с заданным стилем, шириной и цветом
				hPenOld = (HPEN)SelectObject(hdc, hPen1);

				MoveToEx(hdc, 150, 50, NULL);//устанавливает текущей позицией указанную точку
				LineTo(hdc, 450, 50);
				LineTo(hdc, 480, 150);
				LineTo(hdc, 120, 150);
				LineTo(hdc, 150, 50);
				hBrush = CreateSolidBrush(RGB(150, 0, 150));
				hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);
				Rectangle(hdc, 120, 150, 480, 210); //корпус
				hBrush = CreateSolidBrush(RGB(150, 150, 150));//заливка шин
				hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);
				Rectangle(hdc, 130, 210, 150, 240);//отрисовка шин
				Rectangle(hdc, 470, 210, 450, 240);
				hBrush = CreateSolidBrush(RGB(255, 255, 0));//закраска фар
				hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);
				Ellipse(hdc, 140, 165, 170, 195);
				Ellipse(hdc, 430, 165, 460, 195);
				SelectObject(hdc, hBrushOld);
				DeleteObject(hBrush);
				hPen1 = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
				hPenOld = (HPEN)SelectObject(hdc, hPen1);
				MoveToEx(hdc, 300, 150, NULL);
				LineTo(hdc, 450, 120);

				Arc(hdc, 100, 80, 400, 400, 120, 230, 180, 500);
				Arc(hdc, 490, 80, 880, 400, 460, 230, 550, 500);

				POINT poly1[5], poly2[5];

				poly1[0].x = 650;
				poly1[0].y = 400;
				poly1[1].x = 655;
				poly1[1].y = 400;
				poly1[2].x = 655;
				poly1[2].y = 200;
				poly1[3].x = 650;
				poly1[3].y = 200;
				poly1[4].x = 650;
				poly1[4].y = 400;

				Polyline(hdc, poly1, 5);
				hBrush = CreateSolidBrush(RGB(255, 255, 255));
				hPen1 = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
				SelectObject(hdc, hBrush);
				SelectObject(hdc, hPen1);
				Ellipse(hdc, 611, 130, 690, 211);

				poly2[0].x = 300;
				poly2[0].y = 170;
				poly2[1].x = 300;
				poly2[1].y = 180;
				poly2[2].x = 290;
				poly2[2].y = 190;
				poly2[3].x = 300;
				poly2[3].y = 180;
				poly2[4].x = 310;
				poly2[4].y = 190;

				hPen1 = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
				SelectObject(hdc, hPen1);
				Polyline(hdc, poly2, 5);

				hBrush = CreateSolidBrush(RGB(255, 0, 0));
				hPen1 = CreatePen(PS_SOLID, 0, RGB(255, 0, 0));
				SelectObject(hdc, hPen1);
				SelectObject(hdc, hBrush);
				Rectangle(hdc, 620, 160, 680, 180);

				SelectObject(hdc, hPenOld);
				DeleteObject(hPen1);
				
				break;
			}
			case ID_BUTTON_CLEAR:
			{
				HDC hdc = GetDC(hwnd);
				RECT rect;
				GetClientRect(hwnd, &rect);
				rect.bottom -= 80;
				FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
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
		L"GDI Picture (WinAPI Lab_003)",
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

void CreateButtons(HWND hwnd) {
	HWND DrawButton, ClearButton;
	DrawButton = CreateWindow(L"Button", L"Draw", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 290, 500, 100, 50, hwnd, (HMENU)ID_BUTTON_DRAW, NULL, NULL);
	ClearButton = CreateWindow(L"Button", L"Clear", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 410, 500, 100, 50, hwnd, (HMENU)ID_BUTTON_CLEAR, NULL, NULL);
}