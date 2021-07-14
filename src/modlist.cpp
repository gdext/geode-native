#include "modlist.hpp"

ModList* current_modlist = nullptr;

GEODE_HANDLE* GEODE_Init(const char* name) {
    if (current_modlist == nullptr) return nullptr;
    auto mod = current_modlist->getMod(name);

    if (mod == nullptr) return nullptr;

    mod->gHandle = new GEODE_HANDLE();
    return mod->gHandle;
}

ModList::ModList() {};

void ModList::addMod(std::string name, std::string path) {
    mods.push_back({name, path, false});
}

Mod* ModList::getMod(std::string name) {
    for (int i = 0; i < mods.size(); i++)
        if (mods[i].name == name)
            return &mods[i];

    return nullptr;
}

Mod* ModList::getMod(int index) {
    return &mods[index];
}

bool ModList::loadMod(std::string mod) {
    auto m = getMod(mod);
    if (m == nullptr) return false;

    return m->load();
}

bool ModList::unloadMod(std::string mod) {
    auto m = getMod(mod);
    if ( m == nullptr ) return false;

    auto handle = m->gHandle;
    if (handle == nullptr) return false;
    if (handle->disable_callback == nullptr) return false;

    handle->disable_callback();
    return true;
}

int ModList::getModCount() {
    return mods.size();
}

bool Mod::load() {
    if (loaded) return false;

    hModule = LoadLibraryA(path.c_str());

    loaded = true;
    return hModule != NULL;
}