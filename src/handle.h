#include <Windows.h>

typedef void(*GEODE_DISABLE)();

struct GEODE_HANDLE {
    GEODE_DISABLE disable_callback = nullptr;
};