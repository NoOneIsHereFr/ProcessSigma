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
#include "../h/main.h"
#include "../h/tray.h"

// hotkey_thread function prototype
// The reason for this function is to create an own therad to listen for hotkeys. why this is not done in the main thread is because the main thread is blocked by the wait function.
unsigned __stdcall hotkey_thread(void *arg);

bool htQUIT = FALSE;

char *conFiS(float fInput) {
    static char buffer[50];
    snprintf(buffer, sizeof(buffer), "%.2f", fInput);
    return buffer;
}

char *conFiI(int iInput) {
    static char buffer[50];
    snprintf(buffer, sizeof(buffer), "%d", iInput);
    return buffer;
}

int main() {
    system("title THREADASSASSIN");
    system("cls");
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
    
    HINSTANCE hInstance = GetModuleHandle(NULL);

    // Thread creation
    verbosemsg("Starting thread", "inf");
    if (1) { // always true, just to keep the structure
        // Start tray (blocks until exit)
        startT(hInstance, autostart_status);

        verbosemsg("Thread creation finished", "suc");
    }
    
    CloseHandle(hMutex);
    return 0;
}

unsigned __stdcall hotkey_thread(void *arg) {
    verbosemsg("Hotkey thread started", "suc");
    int pressed = 0;
    int counter = 0;
    while (htQUIT == FALSE) {
        if (counter++ % 100 == 0) { // Print every ~1s (100*10ms)
            verbosemsg("Listening for hotkeys", "inf");
        }
        SHORT altState = GetAsyncKeyState(VK_MENU);
        SHORT f5State = GetAsyncKeyState(VK_F5);
        
        if ((altState & 0x8000) && (f5State & 0x8000)) {
            if (!pressed) {
                pressed = 1;
                verbosemsg("Terminating", "suc");
                killPID();
                Sleep(1000);
            }
        } else {
            pressed = 0;
        }
        Sleep(10);
    }
    return 0;
}
