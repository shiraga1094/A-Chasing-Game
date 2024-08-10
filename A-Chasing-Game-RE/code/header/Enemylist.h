#ifndef _Enemylist_
#define _Enemylist_

#include <iostream>
#include <deque>

#include "Enemy.h"

class Enemylist {
private:
	std::deque<Enemy> enemylist;
	std::deque<std::pair<int, int>> targetlist;
public:
	Enemylist();
	void clear();
	int size();
	void push_back(std::pair<int, int> pos, int type, MapData* mapdata);
	void setScatter(int pos, int tmp);
	void Move(int pos, std::pair<int, int> target);
	std::pair<int, int> givePos(int pos);
	std::pair<int, int> givelastPos(int pos);
	std::pair<int, int> givehistoryPos(int pos);
	void setPos(int P, std::pair<int, int> pos, bool isRand=false);
	bool checkisDead(int pos);
	bool checkisUnDead(int pos);
	void erase(int pos);
	void setFiction(int pos);
	bool isFiction(int pos);
	void setParalyze(int pos, int tmp);
	int checkisParalyze(int pos);
	void setUnDead(int pos);
	void toDead(int pos);
	void setCountDown(int pos, int tmp);
	bool checkCountDown(int pos);
	void ResetHistory(int pos);
	void SetEnemyType(int pos, EnemyType enemytype);
	void setCaptureItem(int pos);
	void setCaptureScore(int pos);
	
	void inputtarget(std::pair<int, int> pos);
	std::pair<int, int> givetarget();
	void cleartarget();
};



#endif

