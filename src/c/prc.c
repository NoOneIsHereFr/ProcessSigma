#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "..\h\prc.h"
#include "..\h\verbose.h"

int getPID() {
    HWND window = GetForegroundWindow();
    // if no window is found, return -1
    if (window == NULL) {
        verbosemsg("Error: No foreground window.", "err");
        return -1;
    }
    // if the window is not a valid process, return -1
    DWORD pid = 0;
    GetWindowThreadProcessId(window, &pid);
    if (pid == 0) {
        verbosemsg("Error: No process ID found for the foreground window", "err");
        return -1;
    }
    verbosemsg("PID found", "suc");
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
        return -1;
    }
    // if the process cannot be terminated, return -1
    if (TerminateProcess(hProcess, 0) == 0) {
        verbosemsg("Error: Unable to terminate process", "err");
        CloseHandle(hProcess);
        return -1;
    }
    verbosemsg("PID terminated", "suc");
    CloseHandle(hProcess);
    return 0;
}