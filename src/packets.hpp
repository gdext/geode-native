#pragma once

#include <string>
#include <json.hpp>

#include "modlist.hpp"

using json = nlohmann::json;

class OutPacket {
public:
    std::string getJSONString();

protected:
    OutPacket(std::string type);
    json data;

private:
    std::string type;
};

class InPacket {
public:
    InPacket(std::string str);
    InPacket(json data);

protected:
    json data;
};

class OverlayKeyOutP : public OutPacket {
public:
    OverlayKeyOutP();
};

class ConfigInP : public InPacket {
public:
    ConfigInP(std::string str);
    ConfigInP(json data);

    int overlay_key;
};

class ModListInP : public InPacket {
public:
    ModListInP(std::string str);
    ModListInP(json data);

    ModList* modlist;
};

class ModLoadInP : public InPacket {
public:
    ModLoadInP(std::string str);
    ModLoadInP(json data);

    std::string mod;
};

class ModLoadedOutP : public OutPacket {
public:
    ModLoadedOutP(std::string mod, bool success);

    bool success;
    std::string name;
};

class ModUnloadInP : public InPacket {
public:
    ModUnloadInP(std::string str);
    ModUnloadInP(json data);

    std::string mod;
};

class ModUnloadedOutP : public OutPacket {
public:
    ModUnloadedOutP(std::string mod, bool success);

    bool success;
    std::string name;
};