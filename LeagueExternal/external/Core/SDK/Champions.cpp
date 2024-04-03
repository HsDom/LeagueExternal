#include "Champions.h"

namespace Champions {
    std::map<std::string, Champion> champions;

    void Initialize() {
        Champion twitch;
        twitch.Name = "Twitch";
        twitch.AARange = 550;
        twitch.AttackWindup = 0.20192f;
        twitch.QRange = 500;
        twitch.WRange = 950;
        twitch.ERange = 1200;
        twitch.RRange = 1100;

        champions["Twitch"] = twitch;

        Champion jinx;
        jinx.Name = "Jinx";
        jinx.AARange = 550;
        jinx.AttackWindup = 0.16875f;
        jinx.QRange = 0;
        jinx.WRange = 1500;
        jinx.ERange = 925;
        jinx.RRange = 25000;

        champions["Jinx"] = jinx;
    }

    Champion getChampion(const std::string& name) {
        auto it = champions.find(name);
        if (it != champions.end()) {
            return it->second;
        }
        else {
            return Champion();
        }
    }
}
