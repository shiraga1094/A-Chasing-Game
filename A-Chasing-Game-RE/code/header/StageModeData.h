#ifndef _StageModeData_
#define _StageModeData_

#include <iostream>
#include <vector>
#include <fstream>

struct Object {
	int X, Y; //Position
	char Type;
	int EType;
};
class StageModeData {
private:
	std::vector<std::vector<std::string>> Ability_Name;
	std::vector<std::vector<std::vector<std::string>>> Ability_Intro;
	std::vector<std::vector<Object>> stagemodedata;
	int Obstacle_Map[100][100][100];

public:
	StageModeData();
	void WriteIn();
	std::string giveAbility_Name(int level, int now);
	int giveAbilityAmount(int level);
	int giveAbilityLevelLimit();
	int giveObstacle(int MapNo, int x, int y);
	std::vector<std::string> giveAbilityIntro(int level, int now);
};



#endif

