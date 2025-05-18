#ifndef TRAY_H
#define TRAY_H

#include <windows.h>
#include <shellapi.h>
#include <stdbool.h>

// Global tray icon data (used for notifications)
extern NOTIFYICONDATA nid;

// Functions to create and remove the tray icon
void CreateTrayIcon(HWND hwnd, BOOL autostartState);
void RemoveTrayIcon(HWND hwnd);

// Start the tray (message loop)
void startT(HINSTANCE hInstance, BOOL autostartState);

#endif // TRAY_H