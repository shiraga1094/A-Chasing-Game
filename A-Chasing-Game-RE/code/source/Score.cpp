#include "Score.h"

Score::Score(MapData* _mapdata) {
	mapdata = _mapdata;
}
void Score::setPos(int P, std::pair<int, int> pos) {
	if (P == -1) {
		Pos.push_back(pos);
		mapdata->Update_nxtMap(Pos.back(), 'S');
	}
	else {
		Pos[P] = mapdata->giveLegalPos({ 0,0,Width,Height });
		mapdata->Update_nxtMap(Pos[P], 'S');
	}
}
void Score::ResetPos() {
	for (int i = 0; i < Pos.size(); i++) {
		Pos[i] = mapdata->giveLegalPos({ 0,0,Width,Height });
		mapdata->Update_nxtMap(Pos[i], 'S');
	}
}
bool Score::isGetScore(std::pair<int, int> pos) {
	bool K = 0;
	for (int i = 0; i < Pos.size(); i++) {
		if (Pos[i] == pos) {
			setPos(i);
			K = 1;
		}
		mapdata->Update_nxtMap(Pos[i], 'S');
	}
	if (K) return true;
	return false;
}
void Score::deleteScore() {
	for (int i = 0; i < Pos.size(); i++) {
		mapdata->Update_nxtMap(Pos[i], '.');
		Pos.clear();
	}
}
int Score::giveAmount() {
	return Pos.size();
}
std::vector<std::pair<int, int>> Score::givePos() {
	std::vector<std::pair<int, int>> ans;
	for (int i = 0; i < Pos.size(); i++) {
		ans.push_back(Pos[i]);
	}
	return ans;
}