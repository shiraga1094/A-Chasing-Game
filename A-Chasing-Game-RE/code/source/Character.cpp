#include "Character.h"

Character::Character() {
	UnDead = isDead = false;
	Teleport_Range = 1;
}
void Character::Move(InputStatus key) {
	switch (key) {
		case Up:
			Pos.second-=Teleport_Range; break;
		case Left:
			Pos.first-=Teleport_Range; break;
		case Right:
			Pos.first+=Teleport_Range; break;
		case Down:
			Pos.second+=Teleport_Range; break;
	}
	Teleport_Range = 1;
}
void Character::setPos(std::pair<int, int> tmp) {
	Pos = tmp;
}
std::pair<int, int> Character::givePos() {
	return Pos;
}
bool Character::checkisDead() {
	UnDead = false;
	bool ans = isDead; isDead = false;
	return ans;
}
bool Character::isUnDead() {
	return UnDead;
}
void Character::setUnDead() {
	UnDead = true;
}
void Character::toDead() {
	if (UnDead) {
		return;
	}
	else {
		isDead = true;
	}
}
void Character::addTeleport_Range() {
	Teleport_Range++;
}
int Character::giveTeleport_Range() {
	return Teleport_Range;
}

void Character::AI_Move(Score* score, MapData* mapdata) {
	std::pair<int, int> target = { Width / 2, Height / 2 };
	int min_distance = 1e5;
	for (std::pair<int, int> tmp : score->givePos()) {
		if (tmp.first == -1) break;
		if (abs(tmp.first - Pos.first) + abs(tmp.second - Pos.second) < min_distance) {
			min_distance = abs(tmp.first - Pos.first) + abs(tmp.second - Pos.second);
			target = tmp;
		}
	}
	int try_count = 0;
	std::pair<int, int> nxtPos = Pos;
	while (try_count < 3) {
		try_count++;
		nxtPos = AI_Move_Straight(target, try_count);
		if (mapdata->isInDanger(nxtPos))
			continue;
		if (nxtPos.first < 0 || nxtPos.first >= Width || nxtPos.second < 0 || nxtPos.second >= Height)
			continue;
		else
			break;
	}
	while (try_count >= 3 && try_count < 6) {
		try_count++;
		nxtPos = AI_Move_Scatter(target, try_count);
		if (mapdata->isInDanger(nxtPos))
			continue;
		if (nxtPos.first < 0 || nxtPos.first >= Width || nxtPos.second < 0 || nxtPos.second >= Height)
			continue;
		else
			break;
	}
	Pos = nxtPos;
}
std::pair<int, int> Character::AI_Move_Straight(std::pair<int, int> target, int try_count) {
	int D;
	if (std::abs(Pos.first - target.first) == std::abs(Pos.second - target.second)) {
		D = std::rand() % 2;
	}
	else if (std::abs(Pos.first - target.first) < std::abs(Pos.second - target.second)) {
		D = 1;
	}
	else {
		D = 0;
	}
	if (std::abs(Pos.first - target.first) == 0) {
		D = 0;
	}
	else if (std::abs(Pos.second - target.second) == 0) {
		D = 1;
	}
	if (try_count > 1) {
		D ^= 1;
	}
	int dx = (Pos.first < target.first) ? 1 : -1;
	int dy = (Pos.second < target.second) ? 1 : -1;
	return { Pos.first + dx * D, Pos.second + dy * (D ^ 1) };
}
std::pair<int, int> Character::AI_Move_Scatter(std::pair<int, int> target, int try_count) {
	int D = std::rand() % 2, tD[2] = { 1, -1 };
	int dx = (Pos.first < target.first) ? -1 : 1;
	if (Pos.first == target.first) dx = tD[std::rand() % 2];
	int dy = (Pos.second < target.second) ? -1 : 1;
	if (Pos.second == target.second) dy = tD[std::rand() % 2];
	return { Pos.first + dx * D, Pos.second + dy * (D ^ 1) };
}