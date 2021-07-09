#pragma once
#include <Windows.h>

namespace gd {
    extern HWND window;

    HWND getGDWindow();
    HWND waitForGDWindow();
}