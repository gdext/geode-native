#pragma once

#include <string>
#include <json.hpp>

using json = nlohmann::json;

class OutPacket {
public:
    virtual json getJSON();

    std::string getJSONString();

protected:
    OutPacket(std::string type);

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

class ConfigInP : public InPacket {
public:
    ConfigInP(std::string str);
    ConfigInP(json data);

    int overlay_key;
};