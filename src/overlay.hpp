#pragma once

#include <string>

class Overlay {
public:
    static Overlay* startFromExecutable(std::string path);
    static Overlay* startFromCommand(std::string command);
};