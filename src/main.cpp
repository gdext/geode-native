#include "overlay.hpp"
#include "connection.hpp"
#include "packets.hpp"
#include "keyinput.hpp"

#include "gd.hpp"

ConfigInP* current_config = nullptr;
Connection* connection;

void on_packet(json data) {
    auto ptype = data["type"].get<std::string>();

    if (ptype == "config")
        current_config = new ConfigInP(data);
}

std::string getWindowTitle(HWND hwnd) {
    char title[101];
    GetWindowTextA(hwnd, title, 100);

    title[100] = 0;
    return std::string(title);
}

void keyInput(DWORD scancode) {
    if (current_config != nullptr && scancode == current_config->overlay_key)
        if (GetForegroundWindow() == gd::window)
            connection->send(new OverlayKeyOutP());
}

DWORD WINAPI load_thread(LPVOID hModule) {
    auto o = Overlay::startFromCommand("\"C:\\Users\\Hakim\\Documents\\GitHub\\geode-app\\native-run.bat\"");

    for (int i = 0; i < 20; i++) {
        connection = Connection::connect(3264);

        if (connection != nullptr) break;
        Sleep(500);
    }

    if (connection == nullptr) {
        MessageBoxA(NULL, "The Geode mod could not connect to the overlay.", "Connection error!", MB_ICONERROR);
        return 0;
    }

    connection->onPacket = on_packet;

    if (connection == nullptr) return 0;

    keyinput::onKeyInput = keyInput;
    keyinput::init();

    gd::waitForGDWindow();

    while (true) {
        Sleep(10);
        connection->check_for_data();
    }

    return 0;
}

BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        HANDLE _ = CreateThread(0, 0, load_thread, hModule, 0, nullptr);
        if (_) CloseHandle(_);
    }
    return TRUE;
}