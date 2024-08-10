#ifndef _Ability_
#define _Ability_

#include <iostream>
#include <deque>
#include <vector>

#include "MapData.h"
#include "GameData.h"
#include "Character.h"
#include "Enemylist.h"
#include "Item.h"
#include "Score.h"
#include "DrawWindow.h"
#include "Message.h"

class Ability {
private:
	MapData* mapdata;
	GameData* gamedata;
	Character* character;
	Enemylist *enemylist;
	Item* item;
	Score* score;
	DrawWindow* view;
	Message* message;

	int* AbilityNo;

	int Effect[60] = {};
	int Count[60] = {};
	int EnemyCount[60] = {};

	int grid_0[Width][Height] = {};
	std::deque<std::pair<int, int>> Q_0;
	std::deque<std::pair<int, int>> Q_21;
	std::pair<int, int> Cnt_31;
	std::vector<std::string> str_32 = {
		"不得和其他人站在同一列或行上",
		"所有人座標各取XY平均值乘0.4上下取整，位置需在該行或列上",
		"半徑3格內需多於2人"
	};

public:
	Ability(MapData* _mapdata, GameData* _gamedata, Character* _ch, 
		Enemylist* _enemy, DrawWindow* _view, Item* _item, Score* _score, Message* _message);
	void Initial(int *_abilityno);
	void Active();
	void DrawEffect();
	void Before_Passive();
	void Passive();
};


#endif
