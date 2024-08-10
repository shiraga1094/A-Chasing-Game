#include "Enemylist.h"

Enemylist::Enemylist() {
	enemylist.clear();
}
void Enemylist::clear() {
	enemylist.clear();
}
int Enemylist::size() {
	return enemylist.size();
}
void Enemylist::push_back(std::pair<int, int> pos, int type, MapData* mapdata) {
	enemylist.push_back(Enemy(pos, type, mapdata));
}
void Enemylist::setScatter(int pos, int tmp) {
	enemylist[pos].setScatter(tmp);
}
void Enemylist::Move(int pos, std::pair<int, int> target) {
	enemylist[pos].Move(target);
}
std::pair<int, int> Enemylist::givePos(int pos) {
	return enemylist[pos].givePos();
}
std::pair<int, int> Enemylist::givelastPos(int pos) {
	return enemylist[pos].givelastPos();
}
std::pair<int, int> Enemylist::givehistoryPos(int pos) {
	return enemylist[pos].givehistoryPos();
}
void Enemylist::setPos(int P, std::pair<int, int> pos, bool isRand) {
	enemylist[P].setPos(pos, isRand);
}
bool Enemylist::checkisDead(int pos) {
	return enemylist[pos].checkisDead();
}
bool Enemylist::checkisUnDead(int pos) {
	return enemylist[pos].checkisUnDead();
}
void Enemylist::erase(int pos) {
	enemylist.erase(enemylist.begin() + pos);
}
void Enemylist::setFiction(int pos) {
	enemylist[pos].setFiction();
}
bool Enemylist::isFiction(int pos) {
	return enemylist[pos].checkisFiction();
}
void Enemylist::setParalyze(int pos, int tmp) {
	enemylist[pos].setParalyze(tmp);
}
int Enemylist::checkisParalyze(int pos) {
	return enemylist[pos].checkisParalyze();
}
void Enemylist::setUnDead(int pos) {
	enemylist[pos].setUnDead();
}
void Enemylist::toDead(int pos) {
	enemylist[pos].toDead();
}
void Enemylist::setCountDown(int pos, int tmp) {
	enemylist[pos].setCountDown(tmp);
}
bool Enemylist::checkCountDown(int pos) {
	return enemylist[pos].checkCountDown();
}
void Enemylist::ResetHistory(int pos) {
	enemylist[pos].ResetHistory();
}
void Enemylist::SetEnemyType(int pos, EnemyType enemytype) {
	enemylist[pos].SetEnemyType(enemytype);
}
void Enemylist::setCaptureItem(int pos) {
	enemylist[pos].setCaptureItem();
}
void Enemylist::setCaptureScore(int pos) {
	enemylist[pos].setCaptureScore();
}
void Enemylist::inputtarget(std::pair<int, int> pos) {
	targetlist.push_back(pos);
}
void Enemylist::cleartarget() {
	targetlist.clear();
}
std::pair<int, int> Enemylist::givetarget() {
	int P = std::rand() % targetlist.size();
	return targetlist[P];
}




