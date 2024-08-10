#ifndef _Character_
#define _Character_

#include <iostream>
#include "EnumStatus.h"
#include "Score.h"
#include "MapData.h"


class Character {
private:
	std::pair<int, int> Pos;
	int UnDead;
	int Teleport_Range;
	bool isDead;

public:
	Character();
	void Move(InputStatus key);
	void setPos(std::pair<int,int> tmp);
	std::pair<int, int> givePos();
	bool checkisDead();
	bool isUnDead();
	void setUnDead();
	void toDead();
	void addTeleport_Range();
	int giveTeleport_Range();

	void AI_Move(Score* score, MapData* mapdata);
	std::pair<int, int> AI_Move_Straight(std::pair<int, int> target, int try_count);
	std::pair<int, int> AI_Move_Scatter(std::pair<int, int> target, int try_count);
};






#endif
