#ifndef _Ability_
#define _Ability_

#include <iostream>
#include <deque>
#include <vector>
#include <time.h>

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
		"���o�M��L�H���b�P�@�C�Φ�W",
		"�Ҧ��H�y�ЦU��XY�����ȭ�0.4�W�U����A#��m�ݦb�Ӧ�ΦC�W",
		"�b�|3�椺�ݦh��2�H"
	};
	std::vector<std::string> str_47 = {
		"Prelude", "Interlude", "Afterlude"
	};
	std::pair<int, int> knife = { 0,0 };
	std::vector<std::pair<int, int>> enemyknife;
	double standard, now;
	std::pair<int, int> SpecialItem;

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
