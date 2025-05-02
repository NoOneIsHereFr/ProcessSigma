// AUTHOR: GitHub: @NoOneIsHereFr
// DATE: 2025-05-01
// NAME: PROCCESSSIGMA
/*  
                This program is a simple Windows application that creates a thread to listen for a hotkey (alt+f5).
                It uses the Windows API to create a thread and handle hotkey events.
                The program also includes a verbose message function to print messages with different colors.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <winreg.h>
#include <time.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <stdbool.h>

#include "../h/autostart.h"
#include "../h/verbose.h"
#include "../h/prc.h"

#define POSITIVE 1
#define NEGATIVE 0

unsigned __stdcall hotkey_thread(void *arg);

int main() {
    #ifdef _WIN32
        verbosemsg("Windows: Supported", "suc");
    #else
        verbosemsg("Unix (likley): Not Supported", "err");
        return 1;
    #endif

    verbosemsg("Starting thread", "inf");

    verbosemsg("Checking Autostart status", "inf");
    int autostart_status = IsInStarup();
    if (autostart_status == -1) {
        verbosemsg("Autostart: ERROR; Quitting", "err");
        exit(1);
    } else if (autostart_status == 1) {
        verbosemsg("Autostart: ENABLED", "suc");
    } else {
        verbosemsg("Autostart: DISABLED", "war");
    }

    // Start the hotkey detection thread
    HANDLE thread_handle;
    thread_handle = (HANDLE)_beginthreadex(NULL, 0, hotkey_thread, NULL, 0, NULL);
    if (thread_handle == 0) {
        verbosemsg("Failed to create thread", "err");
        return 1;
    }

    // Run the tray icon logic (optional)
    HINSTANCE hInstance = GetModuleHandle(NULL);
    startT(hInstance, autostart_status);

    // Wait for the hotkey thread to finish (it won't unless the program exits)
    WaitForSingleObject(thread_handle, INFINITE);

    CloseHandle(thread_handle);
    verbosemsg("Thread creation finished", "suc");

    return 0;
}

unsigned __stdcall hotkey_thread(void *arg) {
    verbosemsg("Hotkey thread started", "suc");
    while (1) {
        verbosemsg("Listening for hotkeys", "inf");
        if (GetAsyncKeyState(VK_F5) & 0x8000) {
            verbosemsg("Alt F5 pressed", "suc");
            killPID();
            Sleep(1000);
        }
        Sleep(100);
    }

    return 0;
}