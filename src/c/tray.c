#include <windows.h>
#include "../h/autostart.h"
#include "../h/verbose.h"
#include "../h/update.h"

#define WM_TRAYICON (WM_USER + 1)

NOTIFYICONDATA nid;
HMENU hMenu;

float version = 0.4;

void ToggleAutostart() {
    BOOL isChecked = GetMenuState(hMenu, 1, MF_BYCOMMAND) & MF_CHECKED;
    SetMenuItemInfo(hMenu, 1, TRUE, &(MENUITEMINFO){
        .cbSize = sizeof(MENUITEMINFO),
        .fMask = MIIM_STATE,
        .fState = isChecked ? MFS_UNCHECKED : MFS_CHECKED,
    });
    toggle_autostart();
}

void ShowContextMenu(HWND hwnd) {
    POINT pt;
    GetCursorPos(&pt);
    SetForegroundWindow(hwnd);
    TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
    PostMessage(hwnd, WM_NULL, 0, 0);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    verbosemsg("WindowProc", "inf");
    switch (uMsg) {
        case WM_TRAYICON:
            if (lParam == WM_RBUTTONDOWN || lParam == WM_LBUTTONDOWN) {
                ShowContextMenu(hwnd);
            }
            break;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case 1: // Autostart checkbox
                    ToggleAutostart();
                    break;
                case 2: // Check for updates button
                    verbosemsg("Checking for updates", "inf");
                    bool avalible = check_for_update(&version);
                    if (avalible == TRUE) {
                        verbosemsg("Update available", "suc");
                        system("start https://github.com/NoOneIsHereFr/ProcessSigma/releases");
                    } else {
                        verbosemsg("No updates available", "inf");
                    }
                    break;
                    
                case 3: // Quit button
                    Shell_NotifyIcon(NIM_DELETE, &nid);
                    PostQuitMessage(0);
                    verbosemsg("Quitting", "inf");
                    verbosemsg("Cleanup done", "suc");
                    exit(0);
                    break;
            }
            break;
        case WM_DESTROY:
            Shell_NotifyIcon(NIM_DELETE, &nid);
            PostQuitMessage(0);
            verbosemsg("Quitting", "inf");
            verbosemsg("Cleanup done", "suc");
            exit(0);
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void CreateTrayIcon(HWND hwnd, BOOL autostartState) {
    verbosemsg("Making tray icon", "inf");
    bool autostartEnabled = autostartState;
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    lstrcpy(nid.szTip, TEXT("PrcSigma"));
    Shell_NotifyIcon(NIM_ADD, &nid);

    verbosemsg("Made tray icon", "inf");
    hMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_STRING | (autostartEnabled ? MF_CHECKED : 0), 1, TEXT("Autostart"));
    AppendMenu(hMenu, MF_STRING, 2, TEXT("Check for Updates"));
    AppendMenu(hMenu, MF_STRING, 3, TEXT("Quit"));
}

int startT(HINSTANCE hInstance, bool autostartstatus) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("TrayIconClass");

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, TEXT("PrcSigma"), 0, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);

    CreateTrayIcon(hwnd, autostartstatus);

    MSG msg;
    while (TRUE) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Sleep(100);
    }

    return 0;
}