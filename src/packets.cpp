#include "packets.hpp"

std::string OutPacket::getJSONString() {
    auto e = data.dump();
    return e;
}

OutPacket::OutPacket(std::string type) : type(type) {
    data = {{"type", type}};
}

InPacket::InPacket(std::string str) : data(str) {}

InPacket::InPacket(json data) : data(data) {}

ConfigInP::ConfigInP(std::string str) : ConfigInP(json::parse(str)) {}

ConfigInP::ConfigInP(json data) : InPacket(data) {
    overlay_key = data["overlay_key"].get<int>();
}

OverlayKeyOutP::OverlayKeyOutP() : OutPacket("overlay_key") {}

ModListInP::ModListInP(std::string str) : ModListInP(json::parse(str)) {}

ModListInP::ModListInP(json data) : InPacket(data) {
    auto mods = data["mods"];

    modlist = new ModList();

    for (auto& element : mods)
        modlist->addMod(element[0].get<std::string>(), element[1].get<std::string>());
}

ModLoadInP::ModLoadInP(std::string str) : ModLoadInP(json::parse(str)) {}

ModLoadInP::ModLoadInP(json data) : InPacket(data) {
    mod = data["name"].get<std::string>();
}

ModLoadedOutP::ModLoadedOutP(std::string mod, bool success) : OutPacket("modloaded"), success(success), name(mod) {
    data["name"] = name;
    data["success"] = success;
}

ModUnloadInP::ModUnloadInP(std::string str) : ModUnloadInP(json::parse(str)) {}

ModUnloadInP::ModUnloadInP(json data) : InPacket(data) {
    mod = data["name"].get<std::string>();
}

ModUnloadedOutP::ModUnloadedOutP(std::string mod, bool success) : OutPacket("modunloaded"), success(success), name(mod) {
    data["name"] = name;
    data["success"] = success;
}