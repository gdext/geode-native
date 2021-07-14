#pragma once

#include <string>
#include <vector>

#include <Windows.h>

#include "handle.h"

class ModList;

extern ModList* current_modlist;

extern "C" __declspec(dllexport) GEODE_HANDLE* GEODE_Init(const char* name);

struct Mod {
    std::string name;
    std::string path;
    bool loaded;

    HMODULE hModule;
    GEODE_HANDLE* gHandle = nullptr;

    bool load();
};

class ModList {
public:
    ModList();

    void addMod(std::string name, std::string path);
    Mod* getMod(std::string name);

    Mod* getMod(int index);

    bool loadMod(std::string name);
    bool unloadMod(std::string name);

    int getModCount();

private:
    std::vector<Mod> mods;
};