#pragma once

#include <string>
#include <map>

struct Champion {
    std::string Name;
    float AARange;
    float AttackWindup;
    float QRange;
    float WRange;
    float ERange;
    float RRange;
};

namespace Champions {
    extern std::map<std::string, Champion> champions;

    void Initialize();
    Champion getChampion(const std::string& name);
}
