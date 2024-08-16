#ifndef _Achievement_
#define _Achievement_

#include <iostream>
#include <algorithm>
#include <fstream>
#include "GameData.h"

struct Achieve_Data {
	int Max_Score[4], Max_Item[4], Max_Kill[4], Max_Round[4];
	bool isAchieve;
};
class Achievement {
private:
	Achieve_Data data[100];
	int* AbilityNo;
	int* GameModeNo;
	int key;
	void Encryption();
	bool Decryption();
	void UnlockCheck(GameData *gamedata);
public:
	Achievement(int*__AbilityNo, int *_GameModeNo);
	
	void Input();
	void Output();
	void Update(GameData *gamedata);
	bool CheckisAchieve(int No);
	bool CheckLevelAchieve(int level);
	void CheckSpecialUnlock(GameData* gamedata);
	Achieve_Data giveAchieveData();
};






#endif

