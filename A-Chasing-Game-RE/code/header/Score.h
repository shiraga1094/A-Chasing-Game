#ifndef _Score_
#define _Score_

#include <iostream>
#include <deque>
#include "MapData.h"

class Score {
private:
	std::deque<std::pair<int,int>> Pos;
	MapData* mapdata;
public:
	Score(MapData* _mapdata);
	void ResetPos();
	void setPos(int P = -1, std::pair<int, int> pos={-1, -1});
	bool isGetScore(std::pair<int,int> pos);
	void deleteScore();
	int giveAmount();
	std::vector<std::pair<int, int>> givePos();
	
};




#endif

