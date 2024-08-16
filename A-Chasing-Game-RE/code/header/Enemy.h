#ifndef _Enemy_
#define _Enemy_

#include <iostream>
#include <deque>
#include "MapData.h"

enum EnemyType {
	Random = 0,
	Straight = 1,
	Trap = 2,
	Float = 3,
	Web = 4,
	Scatter = 5,
};
class Enemy {
private:
	MapData* mapdata;
	std::pair<int, int> Pos;
	std::deque<std::pair<int, int>> historyPos;
	int historyPosLimit = 15;
	int WebRange = 4;
	EnemyType enemytype;
	bool isFiction;
	bool isDead;
	bool isUnDead;
	int Scatter_count;
	int MoveLimitPos[4];
	std::pair<int, int> MoveLimitCenter;
	std::pair<int, int> nowtarget;
	int targetturn;
	bool isInMoveLimit(std::pair<int,int> pos);
	int Paralyze_count;
	int CountDown;

	bool CaptureItem, CaptureScore;
	void ChangeTarget(std::pair<int, int> pos);
public:
	Enemy(std::pair<int,int> _Pos, int _enemytype, MapData* _mapdata);
	
	void Move(std::pair<int,int> pos);
	std::pair<int, int> Move_Random(std::pair<int,int> target, int try_count);
	std::pair<int, int> Move_Straight(std::pair<int, int> target, int try_count);
	std::pair<int, int> Move_Trap(std::pair<int, int> target, int try_count);
	std::pair<int, int> Move_Float(std::pair<int, int> target, int try_count);
	std::pair<int, int> Move_Web(std::pair<int, int> target, int try_count);
	std::pair<int, int> Move_Scatter(std::pair<int, int> target, int try_count);

	void setScatter(int ScatterRandom);
	
	void toDead();
	bool checkisDead();
	void setFiction();
	bool checkisFiction();
	bool checkisUnDead();
	void setParalyze(int count);
	int checkisParalyze();
	void setUnDead();
	void setPos(std::pair<int,int> tmp, bool isRand);
	void setCountDown(int P);
	bool checkCountDown();
	void SetEnemyType(EnemyType _enemytype);
	std::pair<int, int> givePos();
	std::pair<int, int> givelastPos();
	std::pair<int, int> givehistoryPos();
	void ResetHistory();
	void setCaptureItem();
	void setCaptureScore();
};







#endif
