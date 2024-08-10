#include "StageModeData.h"

StageModeData::StageModeData() {

}
void StageModeData::WriteIn() {
    std::ifstream ifs("StageModeData.txt", std::ios::in);
    std::vector<Object> tmp;
    for (int i = 0; i <= 100; i++) stagemodedata.push_back(tmp);
    int No, ObjectAmount;
    while (ifs >> No >> ObjectAmount) {
        Object Ob;
        tmp.clear();
        if (ObjectAmount == -1) {
            char ch;
            for (int y = 0; y < 21; y++) {
                for (int x = 0; x < 21; x++) {
                    ifs >> ch;
                    if (ch == '1') Obstacle_Map[No][x][y] = 1;
                }
            }
            continue;
        }
        while (ObjectAmount--) {
            ifs >> Ob.X >> Ob.Y >> Ob.Type;
            if (Ob.Type == 'E') ifs >> Ob.EType;
            tmp.push_back(Ob);
        }
        stagemodedata[No] = tmp;
    }
    ifs.close();
    std::ifstream ifs2("AbilityName.chg", std::ios::in);
    int T; std::string SS;
    for (int i = 0; ifs2 >> T; i++) {
        Ability_Name.push_back({});
        Ability_Intro.push_back({});
        while (T--) {
            ifs2 >> SS;
            Ability_Name[i].push_back(SS);
            Ability_Intro[i].push_back({});
            while (ifs2 >> SS && SS != "##") {
                Ability_Intro[i].back().push_back(SS);
            }
        }
    }
    ifs2.close();
}
std::string StageModeData::giveAbility_Name(int level, int now) {
    return Ability_Name[level][now];
}
int StageModeData::giveAbilityAmount(int level) {
    return Ability_Name[level].size();
}
int StageModeData::giveAbilityLevelLimit() {
    return Ability_Name.size();
}
int StageModeData::giveObstacle(int MapNo, int x, int y) {
    return Obstacle_Map[MapNo][x][y];
}
std::vector<std::string> StageModeData::giveAbilityIntro(int level, int now) {
    return Ability_Intro[level][now];
}