// AUTHOR: GitHub: @NoOneIsHereFr
// DATE: 2025-05-01
// NAME: THREADASSASSIN
/*  
                This program is a simple Windows application that creates a thread to listen for a hotkey (alt+f5).
                It uses the Windows API to create a thread and handle hotkey events.
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

// hotkey_thread function prototype
// The reason for this function is to create an own therad to listen for hotkeys. why this is not done in the main thread is because the main thread is blocked by the wait function.
unsigned __stdcall hotkey_thread(void *arg);

int main() {
    // Mutex to prevent multiple instances of the program
    verbosemsg("Creating mutex", "inf");
    HANDLE hMutex = CreateMutex(NULL, TRUE, "Global\\THREADASSASSIN_Mutex");
    if (hMutex == NULL) {
        verbosemsg("Failed to create mutex", "err");
        return 1;
    }
    
    // Check if the mutex already exists
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        verbosemsg("Another instance is already running", "err");
        CloseHandle(hMutex);
        return 1;
    }
    
    // Check if the program is running in a console window
    if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
        HWND hwnd = GetConsoleWindow();
        if (hwnd != NULL) {
            ShowWindow(hwnd, SW_HIDE);
        }
    }
    
    // Check if the program is running in a Windows environment
    #ifdef _WIN32
        verbosemsg("Windows: Supported", "suc");
    #else
        verbosemsg("Unix (likley): Not Supported", "err");
        return 1;
    #endif
    
    verbosemsg("Starting thread", "inf");
    
    // Thread creation
    HANDLE thread_handle;
    thread_handle = (HANDLE)_beginthreadex(NULL, 0, hotkey_thread, NULL, 0, NULL);
    if (thread_handle == 0) {
        verbosemsg("Failed to create thread", "err");
        return 1;
    }

    // Autostart check
    verbosemsg("Checking Autostart status", "inf");
    int autostart_status = IsInStartup();
    if (autostart_status == -1) {
        verbosemsg("Autostart: ERROR; Quitting", "err");
        exit(0);
    } else if (autostart_status == 1) {
        verbosemsg("Autostart: ENABLED", "suc");
    } else {
        verbosemsg("Autostart: DISABLED", "war");
    }

    // Starting actual instance
    verbosemsg("Thread creation finished", "suc");
    HINSTANCE hInstance = GetModuleHandle(NULL);
    startT(hInstance, autostart_status);

    WaitForSingleObject(thread_handle, INFINITE);
    CloseHandle(thread_handle);
    
    return 0;
}

unsigned __stdcall hotkey_thread(void *arg) {
        verbosemsg("Hotkey thread started", "suc");
        while (1) {
            verbosemsg("Listening for hotkeys", "inf");
            // Check if Alt + F5 is pressed
            SHORT altState = GetAsyncKeyState(VK_MENU);
            SHORT f5State = GetAsyncKeyState(VK_F5);
            SHORT crtlState = GetAsyncKeyState(VK_CONTROL);
            
            if ((altState & 0x8000) && (f5State & 0x8000)) {
                // Check if Ctrl is also pressed
                if (crtlState & 0x8000) {
                    verbosemsg("Ctrl + Alt + F5 pressed", "suc");
                    killPID();
                } else {
                verbosemsg("Alt + F5 pressed", "suc");
                killPID();
                Sleep(1000);
            }
            Sleep(10);
        }
        return 0;
    }
}
