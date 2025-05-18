#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <strsafe.h>
#include <shellapi.h>

#include "..\h\prc.h"
#include "..\h\verbose.h"

// Forward declaration for tray icon handle
extern NOTIFYICONDATA nid;

// Show a native Windows balloon notification
void sToat(const char *title, const char *message) {
    NOTIFYICONDATA notifyData = nid;
    notifyData.uFlags |= NIF_INFO;
    notifyData.uTimeout = 3000;
    StringCchCopyA(notifyData.szInfoTitle, ARRAYSIZE(notifyData.szInfoTitle), title);
    StringCchCopyA(notifyData.szInfo, ARRAYSIZE(notifyData.szInfo), message);
    notifyData.dwInfoFlags = NIIF_INFO;
    notifyData.hWnd = nid.hWnd;
    notifyData.uID = nid.uID;
    BOOL result = Shell_NotifyIconA(NIM_MODIFY, &notifyData);
    if (!result) {
        verbosemsg("Shell_NotifyIconA failed in sToat", "err");
    } else {
        verbosemsg("Shell_NotifyIconA succeeded in sToat", "suc");
    }
}

int getPID() {
    HWND window = GetForegroundWindow();
    // if no window is found, return -1
    if (window == NULL) {
        verbosemsg("Error: No foreground window.", "err");
        sToat("THREADASSASIN", "No foreground window.");
        return -1;
    }
    // if the window is not a valid process, return -1
    DWORD pid = 0;
    GetWindowThreadProcessId(window, &pid);
    if (pid == 0) {
        verbosemsg("Error: No process ID found for the foreground window", "err");
        sToat("THREADASSASIN", "No process ID found for the foreground window.");
        return -1;
    }
    verbosemsg("PID found", "suc");
    sToat("THREADASSASIN", "Foreground process found.");
    return pid;
}

int killPID() {
    int pid = getPID();
    // if the the returned pid is -1, return -1
    if (pid == -1) {
        return -1;
    }
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    
    // if the process to terminate cannot be opened, return -1
    if (hProcess == NULL) {
        verbosemsg("Error: Unable to open process for termination", "err");
        sToat("THREADASSASIN", "Unable to open process for termination.");
        return -1;
    }
    // if the process cannot be terminated, return -1
    if (TerminateProcess(hProcess, 0) == 0) {
        verbosemsg("Error: Unable to terminate process", "err");
        sToat("THREADASSASIN", "Unable to terminate process.");
        CloseHandle(hProcess);
        return -1;
    }
    verbosemsg("PID terminated", "suc");
    sToat("THREADASSASIN", "Process terminated.");
    CloseHandle(hProcess);
    return 0;
}