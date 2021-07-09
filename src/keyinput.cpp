#include "keyinput.hpp"

KeyInputCallback keyinput::onKeyInput = 0;

HHOOK keyboardHook;

LRESULT CALLBACK keyboardEvent (int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && ( wParam == WM_SYSKEYDOWN || wParam == WM_KEYDOWN )) {
        KBDLLHOOKSTRUCT hooked_key = *((KBDLLHOOKSTRUCT*)lParam);
        if (keyinput::onKeyInput != 0) keyinput::onKeyInput(hooked_key.scanCode);
    }

    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

DWORD WINAPI keyInputThread(LPVOID) {
    HINSTANCE ins = GetModuleHandle(NULL);

    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardEvent, ins, NULL);

    MSG message;
    while (GetMessage(&message, NULL, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    UnhookWindowsHookEx(keyboardHook);
    return 0;
}

void keyinput::init() {
    DWORD dwThread;
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)keyInputThread, NULL, NULL, &dwThread);
}