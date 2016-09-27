#include <Windows.h>
#define ID_TEXT_BOX 101
#define ID_LIST_BOX1 102
#define ID_LIST_BOX2 103
#define ID_ADD_BUTTON 104
#define ID_CLEAR_BUTTON 105
#define ID_TO_RIGHT_BUTTON 106
#define ID_DELETE_BUTTON 107

LPCWSTR g_szClassName = L"myWindowClass";
HWND TextBox, ListBox1, ListBox2, AddButton, ClearButton, ToRightButton, DeleteButton = NULL;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
			TextBox = CreateWindow(L"EDIT", L"", WS_BORDER | WS_CHILD | WS_VISIBLE, 200, 40, 400, 30, hwnd, (HMENU) ID_TEXT_BOX, NULL, NULL);
			if (TextBox == NULL)
				MessageBox(hwnd, L"Could not create edit box.", L"Error", MB_OK | MB_ICONERROR);
			ListBox1 = CreateWindow(L"LISTBOX", L"", WS_BORDER | WS_CHILD | WS_VISIBLE, 80, 100, 300, 300, hwnd, (HMENU) ID_LIST_BOX1, NULL, NULL);
			if (ListBox1 == NULL)
				MessageBox(hwnd, L"Could not create list box 1.", L"Error", MB_OK | MB_ICONERROR);
			ListBox2 = CreateWindow(L"LISTBOX", L"", WS_BORDER | WS_CHILD | WS_VISIBLE, 420, 100, 300, 300, hwnd, (HMENU) ID_LIST_BOX2, NULL, NULL);
			if (ListBox2 == NULL)
				MessageBox(hwnd, L"Could not create list box 2.", L"Error", MB_OK | MB_ICONERROR);
			AddButton = CreateWindow(L"Button", L"Add", WS_BORDER | WS_CHILD | WS_VISIBLE, 100, 420, 100, 30, hwnd, (HMENU) ID_ADD_BUTTON, NULL, NULL);
			if (AddButton == NULL)
				MessageBox(hwnd, L"Could not create AddButton.", L"Error", MB_OK | MB_ICONERROR);
			ClearButton = CreateWindow(L"Button", L"Clear", WS_BORDER | WS_CHILD | WS_VISIBLE, 250, 420, 100, 30, hwnd, (HMENU) ID_CLEAR_BUTTON, NULL, NULL);
			if (ClearButton == NULL)
				MessageBox(hwnd, L"Could not create ClearButton.", L"Error", MB_OK | MB_ICONERROR);
			DeleteButton = CreateWindow(L"Button", L"Delete", WS_BORDER | WS_CHILD | WS_VISIBLE, 450, 420, 100, 30, hwnd, (HMENU) ID_DELETE_BUTTON, NULL, NULL);
			if (DeleteButton == NULL)
				MessageBox(hwnd, L"Could not create DeleteButton.", L"Error", MB_OK | MB_ICONERROR);
			ToRightButton = CreateWindow(L"Button", L"To Right", WS_BORDER | WS_CHILD | WS_VISIBLE, 600, 420, 100, 30, hwnd, (HMENU) ID_TO_RIGHT_BUTTON, NULL, NULL);
			if (ToRightButton == NULL)
				MessageBox(hwnd, L"Could not create ToRightButton.", L"Error", MB_OK | MB_ICONERROR);
			break;	

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case ID_ADD_BUTTON:
				{
					int count = GetWindowTextLength(GetDlgItem(hwnd, ID_TEXT_BOX));
					if (count > 0)
					{
						TCHAR* buf;
						buf = (TCHAR*)GlobalAlloc(GPTR, sizeof(TCHAR) * (count + 1));
						GetDlgItemText(hwnd, ID_TEXT_BOX, buf, count + 1);
						if (SendDlgItemMessage(hwnd, ID_LIST_BOX1, LB_FINDSTRINGEXACT, NULL, (LPARAM)buf) == LB_ERR)
							SendDlgItemMessage(hwnd, ID_LIST_BOX1, LB_ADDSTRING, NULL, LPARAM(buf));
						SetDlgItemText(hwnd, ID_TEXT_BOX, L"");
						GlobalFree(buf);
					}
					break;
				}
				case ID_CLEAR_BUTTON:
				{
					SendDlgItemMessage(hwnd, ID_LIST_BOX1, LB_RESETCONTENT, NULL, NULL);
					SendDlgItemMessage(hwnd, ID_LIST_BOX2, LB_RESETCONTENT, NULL, NULL);

					break;
				}
				case ID_DELETE_BUTTON:
				{
					int ind = SendDlgItemMessage(hwnd, ID_LIST_BOX1, LB_GETCURSEL, NULL, NULL);
					if (ind != LB_ERR)
						SendDlgItemMessage(hwnd, ID_LIST_BOX1, LB_DELETESTRING, ind, NULL);

					ind = SendDlgItemMessage(hwnd, ID_LIST_BOX2, LB_GETCURSEL, NULL, NULL);
					if (ind != LB_ERR)
						SendDlgItemMessage(hwnd, ID_LIST_BOX2, LB_DELETESTRING, ind, NULL);

					break;
				}
				case ID_TO_RIGHT_BUTTON:
					int ind = SendDlgItemMessage(hwnd, ID_LIST_BOX1, LB_GETCURSEL, NULL, NULL);
					if (ind != LB_ERR)
					{
						int len = SendDlgItemMessage(hwnd, ID_LIST_BOX1, LB_GETTEXTLEN, ind, NULL);
						TCHAR* buf = (TCHAR*)GlobalAlloc(GPTR, sizeof(TCHAR) * (len + 1));
						SendDlgItemMessage(hwnd, ID_LIST_BOX1, LB_GETTEXT, ind, (LPARAM)buf);
						if (SendDlgItemMessage(hwnd, ID_LIST_BOX2, LB_FINDSTRINGEXACT, NULL, (LPARAM)buf) == LB_ERR)
							SendDlgItemMessage(hwnd, ID_LIST_BOX2, LB_ADDSTRING, NULL, (LPARAM)buf);
						GlobalFree(buf);
					}
					else
					{
						MessageBox(hwnd, L"No item selected!", L"Error", MB_OK | MB_ICONERROR);
					}
					break;
			}
			break;
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
		L"Controls (WinAPI Lab_002)",
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