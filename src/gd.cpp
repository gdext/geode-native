#include "gd.hpp"

#include <string>

HWND found_window = NULL;
DWORD current_process = NULL;

HWND gd::window = NULL;

BOOL CALLBACK enumWindowsProc(HWND hwnd, LPARAM) {
    if (found_window != NULL) return false;

    DWORD parent_process;
    GetWindowThreadProcessId(hwnd, &parent_process);

    if (parent_process == current_process) {
        char title_text[15];
        GetWindowTextA(hwnd, title_text, 14);

        title_text[14] = 0;

        if (strcmp(title_text, "Geometry Dash") == 0) {
            found_window = hwnd;
            return false;
        }
    }

    return true;
}

HWND gd::getGDWindow() {
    found_window = NULL;
    current_process = GetCurrentProcessId();

    EnumWindows(enumWindowsProc, NULL);
    return found_window;
}

HWND gd::waitForGDWindow() {
    while (true) {
        Sleep(200);

        window = getGDWindow();
        if (window != NULL) return window;
    }
}