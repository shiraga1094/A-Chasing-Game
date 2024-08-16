#include "GameData.h"

GameData::GameData() {
	Character_Initial_Pos = { 10, 10 };
	EnemyAmount = 2;
	Enemy_Initial_Pos = { {4,4}, {4, 14} };
	Enemy_Initial_Type = { 0,1 };

	Enemy_count = EnemyAmount;
	Life_count = 3;
	Item_count = Score_count = 0;
	Kill_count = 0;
	Max_Item_count = Max_Score_count = 0;
	Enemy_getItem_count = 0;

	isGetItem = false;
	isGetScore = false;
	isPerfect = true;

	lastkey = Space;
}