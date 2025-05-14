#include <windows.h>
#include "../h/autostart.h"
#include "../h/verbose.h"
#include "../h/update.h"
#include "../h/main.h"
#include <tchar.h>

#define WM_TRAYICON (WM_USER + 1)

NOTIFYICONDATA nid;
HMENU hMenu;

float version = 0.7;

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
    switch (uMsg) {
        case WM_TRAYICON:
            if (lParam == WM_RBUTTONUP) {
                POINT pt;
                GetCursorPos(&pt);
                SetForegroundWindow(hwnd);
                TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, NULL);
            }
            break;
            
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case 1: // Autostart checkbox
                    ToggleAutostart();
                    break;
                case 2: // Check for updates button
                    verbosemsg("Checking for updates", "infs");
                    bool avalible = check_for_update(&version);
                    if (avalible == TRUE) {
                        verbosemsg("Update available", "suc");
                        system("start https://github.com/NoOneIsHereFr/ProcessSigma/releases");
                    } else {
                        verbosemsg("No updates available", "suc");
                    }
                    break;
                    
                case 3: // Quit button
                    verbosemsg("Quitting", "war");
                    DestroyWindow(hwnd);
                    break;
            }
            break;
            
        case WM_DESTROY:
            Shell_NotifyIcon(NIM_DELETE, &nid);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void CreateTrayIcon(HWND hwnd, BOOL autostartState) {
    verbosemsg("Making tray icon", "infs");
    bool autostartEnabled = autostartState;
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    lstrcpy(nid.szTip, TEXT("ThreadAssassin"));
    Shell_NotifyIcon(NIM_ADD, &nid);

    verbosemsg("Made tray icon", "suc");
    hMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_STRING | (autostartEnabled ? MF_CHECKED : 0), 1, TEXT("Autostart"));
    
    TCHAR buffer[256];
    _stprintf(buffer, TEXT("Check for Updates // %.1f"), version);
    AppendMenu(hMenu, MF_STRING, 2, buffer);
    
    AppendMenu(hMenu, MF_STRING, 3, TEXT("Quit"));
}

int startT(HINSTANCE hInstance, bool autostartstatus) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("TrayIconClass");

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, TEXT("ThreadAssassin"), 0, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);

    if (!hwnd) {
        verbosemsg("Failed to create window", "err");
        return -1;
    }

    CreateTrayIcon(hwnd, autostartstatus);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}