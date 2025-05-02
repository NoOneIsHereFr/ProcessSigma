#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "..\h\prc.h"
#include "..\h\verbose.h"

int getPID() {
    HWND window = GetForegroundWindow();
    if (window == NULL) {
        verbosemsg("Error: No foreground window.\n", "err");
        return -1;
    }
    DWORD pid = 0;
    GetWindowThreadProcessId(window, &pid);
    if (pid == 0) {
        verbosemsg("Error: No process ID found for the foreground window.\n", "err");
        return -1;
    }
    verbosemsg("PID found", "suc");
    return pid;
}

int killPID() {
    int pid = getPID();
    if (pid == -1) {
        return -1;
    }
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (hProcess == NULL) {
        verbosemsg("Error: Unable to open process for termination.\n", "err");
        return -1;
    }
    if (TerminateProcess(hProcess, 0) == 0) {
        verbosemsg("Error: Unable to terminate process.\n", "err");
        CloseHandle(hProcess);
        return -1;
    }
    verbosemsg("PID terminated", "suc");
    CloseHandle(hProcess);
    return 0;
}