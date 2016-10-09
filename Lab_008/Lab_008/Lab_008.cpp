#include <Windows.h>
#include <TlHelp32.h>

#define ID_LB1 101
#define ID_LB2 102

HINSTANCE hInst;
LPCWSTR g_szClassName = L"myWindowClass";
HWND lb1, lb2;

void GetListOfProcesses();
void GetListOfModules();

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		lb1 = CreateWindow(L"LISTBOX", L"LB1", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL,
			5, 5, 380, 550, hwnd, (HMENU) ID_LB1, hInst, NULL);
		lb2 = CreateWindow(L"LISTBOX", L"LB2", WS_VISIBLE | WS_CHILD | WS_BORDER,
			395, 5, 380, 550, hwnd, (HMENU)ID_LB2, hInst, NULL);
		GetListOfProcesses();

		break;
	}
	case WM_COMMAND:
	{
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
		L"Priority (Lab_008)",
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

void GetListOfProcesses() {
	PROCESSENTRY32 peProcessEntry;
	TCHAR szBuff[1024];
	HANDLE const hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return;
	}

	peProcessEntry.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapshot, &peProcessEntry);

	do {
		wsprintf(szBuff, L"%08X %s\r\n",
			peProcessEntry.th32ProcessID, peProcessEntry.szExeFile);
		SendMessage(lb1, LB_ADDSTRING, NULL, (LPARAM)szBuff);
	} while (Process32Next(hSnapshot, &peProcessEntry));

	CloseHandle(hSnapshot);
}

void GetListOfModules() {
	SendMessage(lb2, LB_RESETCONTENT, NULL, NULL);
	MODULEENTRY32 meModuleEntry;
	TCHAR szBuff[1024];
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, NULL);

	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return;
	}

	meModuleEntry.dwSize = sizeof(MODULEENTRY32);
	Module32First(hSnapshot, &meModuleEntry);
		
	do {
		wsprintf(szBuff, L"%08X, %08X, %s\r\n",
			meModuleEntry.modBaseAddr, meModuleEntry.modBaseSize,
			meModuleEntry.szModule);
		SendMessage(lb2, LB_ADDSTRING, NULL, (LPARAM)szBuff);
	} while (Module32Next(hSnapshot, &meModuleEntry));
}