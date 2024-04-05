#include "Champions.h"

namespace Champions {
    std::map<std::string, Champion> champions;

    void Initialize() {
        Champion twitch;
        twitch.Name = "Twitch";
        twitch.AttackWindup = 0.20192f;
        // Auto Attack
        twitch.AutoAblity.Range = 550;
        twitch.AutoAblity.OriginOffset = 50;
        twitch.AutoAblity.Modifier = 195;
        // Q
        twitch.QAblity.Range = 500;
        twitch.QAblity.OriginOffset = 33;
        twitch.QAblity.Modifier = 115;
        // W
        twitch.WAblity.Range = 950;
        twitch.WAblity.OriginOffset = 130;
        twitch.WAblity.Modifier = 240;
        // E
        twitch.EAblity.Range = 1200;
        twitch.EAblity.OriginOffset = 190;
        twitch.EAblity.Modifier = 320;
        // R
        twitch.RAblity.Range = 0;
        twitch.RAblity.OriginOffset = 0;
        twitch.RAblity.Modifier = 0;

        champions["Twitch"] = twitch;

        Champion jinx;
        jinx.Name = "Jinx";
       //jinx.AARange = 550;
        //jinx.AttackWindup = 0.16875f;
        //jinx.QRange = 0;
        //jinx.WRange = 1500;
        //jinx.ERange = 925;
        //jinx.RRange = 25000;

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
