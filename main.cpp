#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <thread>
#include <chrono>

#include "debug.h"

// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <stdlib.h>
#include <string.h>

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("ScreenIdlerClass");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("ScreenIdler");

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void doExit(int eCode = 0);

bool bexit = false;

int monitors = 1;

HINSTANCE hInst = NULL;
HHOOK kbHook = NULL;

BOOL CALLBACK kbProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0) {
        return CallNextHookEx(kbHook, nCode, wParam, lParam);
    }

    LPKBDLLHOOKSTRUCT keyStruct = (LPKBDLLHOOKSTRUCT)lParam;

    if ((keyStruct->flags >> LLKHF_INJECTED) & 1) {
        // ignore injected events
    }
	else {
        bexit = true;
	}
	return CallNextHookEx(kbHook, nCode, wParam, lParam);

}

BOOL CALLBACK MyInfoEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
    MONITORINFOEX info;
    info.cbSize = sizeof(info);
    if (GetMonitorInfo(hMonitor, &info))
    {

    }

    HWND hWnd = CreateWindowA(
        szWindowClass,
        szTitle,
        WS_MAXIMIZE | WS_POPUP | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL,
        NULL,
        NULL,
        hInst,
        NULL
    );

    if (!hWnd)
    {
        MessageBoxA(NULL,
            _T("Call to CreateWindow failed!"),
            szTitle,
            NULL);

        return 1;
    }

    int x = info.rcMonitor.left;
    int y = info.rcMonitor.top;
    int cx = std::abs(info.rcMonitor.left - info.rcMonitor.right);
    int cy = std::abs(info.rcMonitor.top - info.rcMonitor.bottom);
    SetWindowPos(hWnd, NULL, x, y, cx, cy, SWP_NOREDRAW | SWP_SHOWWINDOW);
    _D("Monitor " << monitors << ": " << cx << "x" << cy << 
        " (" << x << "," << y << ")");
    monitors += 1;
    return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    hInst = hInstance;
    createDbgConsole();

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor = NULL;
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClassEx(&wcex))
    {
        MessageBoxA(NULL,
            _T("Call to RegisterClassEx failed!"),
            szTitle,
            NULL);

        return 1;
    }

    kbHook = SetWindowsHookExA(WH_KEYBOARD_LL, (HOOKPROC)kbProc, NULL, NULL);

    EnumDisplayMonitors(NULL, NULL, MyInfoEnumProc, 0);

    while (GetAsyncKeyState(VK_LBUTTON)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (bexit || GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON) || GetAsyncKeyState(VK_MBUTTON))
            break;
    }
    doExit();
}

void doExit(int eCode)
{
    if (kbHook) {
        UnhookWindowsHookEx(kbHook);
    }
    UnregisterClassA(szWindowClass, hInst);
    ExitProcess(eCode);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_SETCURSOR:
    {
        if (LOWORD(lParam) == HTCLIENT)
        {
            SetCursor(NULL);
        }
        break;
    }
    case WM_DESTROY:
        doExit(1);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}