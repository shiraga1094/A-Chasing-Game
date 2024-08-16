#ifndef _GameData_
#define _GameData_

#include <iostream>
#include <vector>
#include <deque>
#include "EnumStatus.h"

class GameData {
public:
	GameData();
	const int EnemyTypeLimit[4] = { 2,3,4,0 };
	const int EnemyRandomLimit[4] = { 70,50,30,1 << 30 };
	const int EnemyAmountLimit[4] = { 1,2,3,0 };
	const int ItemRandomLimit[4] = { 10,15,20,20 };
	const int ItemAmountLimit[4] = { 2,1,1,1 };
	const int ScatterRandom[4] = { 15, 20, 25, 1 << 30 };

	std::pair<int, int> Character_Initial_Pos;
	int EnemyAmount;
	std::vector<std::pair<int, int>> Enemy_Initial_Pos;
	std::vector<int> Enemy_Initial_Type;
	
	int Enemy_count;
	int Life_count;
	int Item_count;
	int Score_count;
	int Kill_count;
	int Round_count;
	int Max_Item_count;
	int Max_Score_count;
	int Enemy_getItem_count;

	bool isGetItem;
	bool isGetScore;
	bool isPerfect;

	InputStatus lastkey;
};




#endif

