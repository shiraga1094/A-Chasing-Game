#include "Item.h"

Item::Item(MapData* _mapdata) {
	mapdata = _mapdata;
	SetItem = true;
}
void Item::setPos(int P, std::pair<int, int> pos, int S) {
	if (P == -1) {
		Pos.push_back(pos);
		if (S == -1) {
			mapdata->Update_nxtMap(Pos.back(), 'I');
			SetItem = false;
		}
	}
	else {
		Pos[P] = mapdata->giveLegalPos({ 0,0,Width,Height });
		if (S == -1) {
			mapdata->Update_nxtMap(Pos[P], 'I');
			SetItem = false;
		}
	}
}
void Item::ResetPos() {
	for (int i = 0; i < Pos.size(); i++) {
		mapdata->Update_nxtMap(Pos[i], '.');
		Pos[i] = mapdata->giveLegalPos({ 0,0,Width,Height });
		mapdata->Update_nxtMap(Pos[i], 'I');
	}
}
void Item::deleteItem() {
	for (int i = 0; i < Pos.size(); i++) {
		mapdata->Update_nxtMap(Pos[i], '.');
	}
	Pos.clear();
}
bool Item::isGetItem(std::pair<int, int> pos) {
	bool K = 0;
	for (int i = 0; i < Pos.size(); i++) {
		if (Pos[i] == pos) {
			K = 1;
			Pos.erase(Pos.begin() + i);
			i--;
		}
	}
	if (SetItem = true) {
		for (int i = 0; i < Pos.size(); i++) {
			mapdata->Update_nxtMap(Pos[i], 'I');
		}
	}
	else {
		SetItem = true;
	}
	if (K) return true;
	return false;
}
int Item::giveAmount() {
	return Pos.size();
}
std::vector<std::pair<int, int>> Item::givePos() {
	std::vector<std::pair<int, int>> ans;
	for (int i = 0; i < Pos.size(); i++) {
		ans.push_back(Pos[i]);
	}
	return ans;
}
