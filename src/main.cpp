#include "overlay.hpp"
#include "connection.hpp"
#include "packets.hpp"

void on_packet(json data) {
    auto ptype = data["type"].get<std::string>();

    if (ptype == "config") {
        ConfigInP p(data);

        MessageBoxA(NULL, (std::string("overlay_key: ") + std::to_string(p.overlay_key)).c_str(),"Packet Recieved: CONFIG", MB_ICONINFORMATION);
    }
}

DWORD WINAPI load_thread(LPVOID hModule) {
    auto o = Overlay::startFromCommand("\"C:\\Users\\Hakim\\Documents\\GitHub\\geode-app\\native-run.bat\"");

    Connection* con;
    for (int i = 0; i < 10; i++) {
        con = Connection::connect(3264);

        if (con != nullptr) break;
    }

    if (con == nullptr) {
        MessageBoxA(NULL, "The Geode mod could not connect to the overlay.", "Connection error!", MB_ICONERROR);
        return 0;
    }

    con->onPacket = on_packet;

    if (con == nullptr) return 0;

    while (true) {
        Sleep(10);
        con->check_for_data();
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