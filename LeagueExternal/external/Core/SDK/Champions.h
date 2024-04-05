#pragma once
#include "structs.h"
#include <string>
#include <map>

struct Ability {
    float Range;
    float OriginOffset;
    float Modifier;
};


struct Champion {
    std::string Name;
    float AttackWindup;
    Ability AutoAblity;
    Ability QAblity;
    Ability WAblity;
    Ability EAblity;
    Ability RAblity;
};

namespace Champions {
    extern std::map<std::string, Champion> champions;

    void Initialize();
    Champion getChampion(const std::string& name);
}
