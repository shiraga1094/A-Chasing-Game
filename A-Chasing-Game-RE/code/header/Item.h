#ifndef _Item_
#define _Item_

#include <iostream>
#include <deque>
#include <vector>
#include "MapData.h"
#include "CursorControl.h"

class Item {
private:
	std::deque<std::pair<int, int>> Pos;
	MapData* mapdata;
	bool SetItem;
public:
	Item(MapData* _mapdata);
	void setPos(int P = -1, std::pair<int,int> pos={-1, -1}, int S=-1);
	void ResetPos();
	void deleteItem();
	bool isGetItem(std::pair<int,int> pos);
	int giveAmount();
	std::vector<std::pair<int, int>> givePos();
};





#endif
