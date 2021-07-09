#pragma once

#include <Windows.h>

typedef void(*KeyInputCallback)(DWORD);

namespace keyinput {
    void init();

    extern KeyInputCallback onKeyInput;
}