#include "overlay.hpp"
#include "connection.hpp"
#include "packets.hpp"
#include "keyinput.hpp"

#include "gd.hpp"

#include <shlobj.h>

ConfigInP* current_config = nullptr;
Connection* connection;

extern "C" void __declspec(dllexport) GEODE_RequiredImport() {}

void on_packet(json data) {
    auto ptype = data["type"].get<std::string>();

    if (ptype == "config")
        current_config = new ConfigInP(data);
    
    if (ptype == "modlist") {
        ModListInP packet(data);

        current_modlist = packet.modlist;
    }

    if (ptype == "modload") {
        ModLoadInP packet(data);

        if (current_modlist != nullptr)
            connection->send(new ModLoadedOutP(packet.mod, current_modlist->loadMod(packet.mod)));
        else
            connection->send(new ModLoadedOutP(packet.mod, false));
    }

    if (ptype == "modunload") {
        ModUnloadInP packet(data);

        if (current_modlist != nullptr)
            connection->send(new ModUnloadedOutP(packet.mod, current_modlist->unloadMod(packet.mod)));
        else
            connection->send(new ModUnloadedOutP(packet.mod, false));
    }
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
    CHAR my_documents[MAX_PATH];
    HRESULT result = SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

    auto o = Overlay::startFromCommand("\"" + std::string(my_documents) + "\\GitHub\\geode-app\\native-run.bat\"");

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