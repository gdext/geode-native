#include "packets.hpp"

json OutPacket::getJSON() {
    return json("{\"type\":\"" + type + "\"}");
}

std::string OutPacket::getJSONString() {
    return getJSON().dump();
}

OutPacket::OutPacket(std::string type) : type(type) {}

InPacket::InPacket(std::string str) : data(str) {}

InPacket::InPacket(json data) : data(data) {}

ConfigInP::ConfigInP(std::string str) : ConfigInP(json::parse(str)) {}

ConfigInP::ConfigInP(json data) : InPacket(data) {
    overlay_key = data["overlay_key"].get<int>();
}