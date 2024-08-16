#include "Enemy.h"
#include <math.h>

Enemy::Enemy(std::pair<int,int> _Pos, int _enemytype, MapData* _mapdata) {
	mapdata = _mapdata;
	Pos = _Pos;
	switch (_enemytype) {
		case 0:
			enemytype = Random; break;
		case 1:
			enemytype = Straight; break;
		case 2:
			enemytype = Trap; break;
		case 3:
			enemytype = Float; break;
		case 4:
			enemytype = Web; break;
		case 5:
			enemytype = Scatter; break;
	}
	isFiction = isDead = isUnDead = false;
	Scatter_count = Paralyze_count = 0;
	CountDown = -1;
	if (enemytype == Web) {
		MoveLimitPos[0] = std::max(Pos.first - 4, 0);
		MoveLimitPos[1] = std::max(Pos.second - 4, 0);
		MoveLimitPos[2] = std::min(Pos.first + 4, Width-1);
		MoveLimitPos[3] = std::min(Pos.second + 4, Height-1);
		MoveLimitCenter = Pos;
	}
	CaptureItem = CaptureScore = false;
	targetturn = 0;
}
void Enemy::ChangeTarget(std::pair<int, int> pos) {
	if (!targetturn) {
		nowtarget = pos;
		targetturn = 3;
	}
	else {
		targetturn--;
	}
}
bool Enemy::isInMoveLimit(std::pair<int, int> pos) {
	return pos.first >= MoveLimitPos[0] && pos.second >= MoveLimitPos[1] && pos.first <= MoveLimitPos[2] && pos.second <= MoveLimitPos[3];
}
void Enemy::Move(std::pair<int, int> pos) {
	ChangeTarget(pos);
	std::pair<int, int> nxtPos, target = nowtarget;
	int try_count = 0;
	EnemyType tmp_enemytype = enemytype;
	if (CountDown > 0) {
		CountDown--;
		if (CountDown == 0)
			toDead();
	}
	if (Paralyze_count > 0) {
		mapdata->Update_nxtMap(Pos, 'E');
		Paralyze_count--;
		return;
	}
	if (Scatter_count-->0) tmp_enemytype = Scatter;
	while (try_count++ < 3 && Paralyze_count<=0) {
		switch (tmp_enemytype) {
			case Random:
				nxtPos = Move_Random(target, try_count); break;
			case Straight:
				nxtPos = Move_Straight(target, try_count); break;
			case Trap:
				nxtPos = Move_Trap(target, try_count); break;
			case Float:
				nxtPos = Move_Float(target, try_count); break;
			case Web:
				nxtPos = Move_Web(target, try_count); break;
			case Scatter:
				nxtPos = Move_Scatter(target, try_count); break;
		}
		if (enemytype == Web && !isInMoveLimit(nxtPos)) {
			continue;
		}
		if (nxtPos.first < 0 || nxtPos.first >= Width || nxtPos.second<0 || nxtPos.second>=Height) {
			continue;
		}
		if (!CaptureItem && mapdata->checknowMap(nxtPos) == 'I')
			continue;
		if (!CaptureScore && mapdata->checknowMap(nxtPos) == 'S')
			continue;
		if (!mapdata->isInDanger(nxtPos) && !mapdata->checkObstacle(nxtPos)) {
			break;
		}
	}
	historyPos.push_back(Pos);
	if (historyPos.size() > historyPosLimit)
		historyPos.pop_front();
	if (try_count <= 3 && Paralyze_count<=0) {
		Pos = nxtPos;
	}
	if (isFiction==true) {
		mapdata->Update_nxtMap(Pos, 'F');
	}
	else {
		mapdata->Update_nxtMap(Pos, 'E');
	}
}
std::pair<int, int> Enemy::Move_Random(std::pair<int, int> target, int try_count) {
	int D = std::rand() % 2, tD[2] = { 1,-1 };
	int dx = (Pos.first < target.first) ? 1 : -1;
	if (Pos.first == target.first) dx = tD[std::rand() % 2];
	int dy = (Pos.second < target.second) ? 1 : -1;
	if (Pos.second == target.second) dy = tD[std::rand() % 2];
	return { Pos.first + dx * D, Pos.second + dy * (D^1) };
}
std::pair<int, int> Enemy::Move_Straight(std::pair<int, int> target, int try_count) {
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
	return { Pos.first + dx * D, Pos.second + dy * (D^1) };
}
std::pair<int, int> Enemy::Move_Trap(std::pair<int, int> target, int try_count) {
	if (std::abs(Pos.first - target.first) > 3 && std::abs(Pos.second - target.second) > 3) {
		int dx = (Pos.first < target.first) ? 1 : -1;
		if (Pos.first == target.first) dx = 0;
		int dy = (Pos.second < target.second) ? 1 : -1;
		if (Pos.second == target.second) dy = 0;
		target.first += dx * 3; target.second += dy * 3;
		if (target.first < 0) target.first = 0;
		else if (target.first > Width - 1) target.first = Width - 1;
		if (target.second < 0) target.second = 0;
		else if (target.second > Height - 1) target.second = Height - 1;
	}
	return Move_Straight(target, try_count);
}
std::pair<int, int> Enemy::Move_Float(std::pair<int, int> target, int try_count) {
	int DA[2] = { 1, 0 }, DB[2] = { 1, -1 };
	if (std::abs(Pos.first - target.first) > 3 && std::abs(Pos.second - target.second) > 3) {
		int A = rand() % 2, B = rand() % 2;
		return { Pos.first + DA[A] * DB[B], Pos.second + DA[A ^ 1] * DB[B ^ 1] };
	}
	return Move_Straight(target, try_count);
}
std::pair<int, int> Enemy::Move_Web(std::pair<int, int> target, int try_count) {
	if (isInMoveLimit(target)) {
		return Move_Straight(target, try_count);
	}
	else if (std::abs(MoveLimitCenter.first - Pos.first) > 3 || std::abs(MoveLimitCenter.second - Pos.second) > 3) {
		return Move_Straight(MoveLimitCenter, try_count);
	}
	else {
		return Move_Float(target, try_count);
	}
}
std::pair<int, int> Enemy::Move_Scatter(std::pair<int, int> target, int try_count) {
	int D = std::rand() % 2, tD[2] = { 1, -1 };
	int dx = (Pos.first < target.first) ? -1 : 1;
	if (Pos.first == target.first) dx = tD[std::rand() % 2];
	int dy = (Pos.second < target.second) ? -1 : 1;
	if (Pos.second == target.second) dy = tD[std::rand() % 2];
	return { Pos.first + dx * D, Pos.second + dy * (D ^ 1)};
}

void Enemy::setScatter(int ScatterRandom) {
	if (std::rand() % ScatterRandom == 0) {
		Scatter_count = 3;
	}
}
void Enemy::toDead() {
	if (isUnDead) return;
	isDead = true;
}
bool Enemy::checkisDead() {
	bool ans = isDead; isDead = false;
	return ans;
}
void Enemy::setFiction() {
	isFiction = true;
}
bool Enemy::checkisFiction() {
	return isFiction;
}
bool Enemy::checkisUnDead() {
	return isUnDead;
}
void Enemy::setParalyze(int count) {
	Paralyze_count = count;
}
int Enemy::checkisParalyze() {
	return Paralyze_count;
}
void Enemy::setUnDead() {
	isUnDead = true;
}
void Enemy::setPos(std::pair<int, int> tmp, bool isRand) {
	Pos = tmp;
	if (isRand == true) {
		MoveLimitPos[0] = std::max(Pos.first - 4, 0);
		MoveLimitPos[1] = std::max(Pos.second - 4, 0);
		MoveLimitPos[2] = std::min(Pos.first + 4, Width - 1);
		MoveLimitPos[3] = std::min(Pos.second + 4, Height - 1);
		MoveLimitCenter = Pos;
	}
}
void Enemy::setCountDown(int P) {
	CountDown = P;
}
bool Enemy::checkCountDown() {
	return CountDown != -1;
}
void Enemy::SetEnemyType(EnemyType _enemytype) {
	enemytype = _enemytype;
}
std::pair<int, int> Enemy::givePos() {
	return Pos;
}
std::pair<int, int> Enemy::givelastPos() {
	return historyPos.back();
}
std::pair<int, int> Enemy::givehistoryPos() {
	if (historyPos.size() < historyPosLimit) {
		return { -1, -1 };
	}
	return historyPos.front();
}
void Enemy::ResetHistory() {
	historyPos.clear();
}
void Enemy::setCaptureItem() {
	CaptureItem = true;
}
void Enemy::setCaptureScore() {
	CaptureScore = true;
}