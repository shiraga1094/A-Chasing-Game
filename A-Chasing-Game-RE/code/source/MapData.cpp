#include "MapData.h"

MapData::MapData() {
	for (int i = 0; i < Width; i++) {
		for (int s = 0; s < Height; s++) {
			nowMap[i][s] = nxtMap[i][s] = '.';
			DangerMap[i][s] = Obstacle[i][s] = 0;
		}
	}
}
void MapData::inputObstacle(int MapNo, StageModeData* data) {
	for (int x = 0; x < Width; x++) {
		for (int y = 0; y < Height; y++) {
			Obstacle[x][y] = data->giveObstacle(MapNo, x, y);
		}
	}
}
void MapData::Clear() {
	for (int i = 0; i < Width; i++) {
		for (int s = 0; s < Height; s++) {
			nowMap[i][s] = nxtMap[i][s] = '.';
			DangerMap[i][s] = 0;
		}
	}
}
void MapData::Initialize() {
	for (int i = 0; i < Width; i++) {
		for (int s = 0; s < Height; s++) {
			nowMap[i][s] = nxtMap[i][s];
			nxtMap[i][s] = '.';
			DangerMap[i][s] = 0;
		}
	}
}
std::pair<int, int> MapData::giveLegalPos(std::vector<int> border, std::pair<int, int> tmp) {
	int try_count = 0;
	while (try_count++ < 100) {
		int x = std::rand() % Width, y=std::rand()% Height;
		if (tmp.first != -1) {
			if ((x >= tmp.first - 4 && x <= tmp.first + 4) || (y >= tmp.second - 4 && y <= tmp.second + 4))
				continue;
		}
		if (x < border[0] || y < border[1] || x >= border[2] || y >= border[3])
			continue;
		if (checknowMap({ x,y }) == 'S' || checknowMap({ x,y }) == 'I')
			continue;
		if (nxtMap[x][y] != '.')
			continue;
		if (isInDanger({ x,y }) || checkObstacle({x,y}))
			continue;
		return { x,y };
	}
}
char MapData::checknowMap(std::pair<int, int> pos) {
	return nowMap[pos.first][pos.second];
}
bool MapData::checkObstacle(std::pair<int, int> pos) {
	return (Obstacle[pos.first][pos.second]>0);
}
char MapData::checknxtMap(std::pair<int, int> pos) {
	return nxtMap[pos.first][pos.second];
}
void MapData::Update_DangerMap(std::pair<int, int> pos) {
	if (pos.first != -1) {
		DangerMap[pos.first][pos.second]--;
		return;
	}
	for (int r = 0; r < Width; r++) {
		for (int c = 0; c < Height; c++) {
			if (nowMap[r][c] == 'E')
				DangerMap[r][c]++;
		}
	}
}
void MapData::Update_nxtMap(std::pair<int, int> pos, char ch) {
	nxtMap[pos.first][pos.second] = ch;
	if (ch == 'E') DangerMap[pos.first][pos.second]++;
}
void MapData::Update_ObstacleMap(std::pair<int, int> pos, bool isSet) {
	if (isSet) {
		Obstacle[pos.first][pos.second]++;
	}
	else {
		Obstacle[pos.first][pos.second]--;
	}
}
bool MapData::isInDanger(std::pair<int, int> pos) {
	return (DangerMap[pos.first][pos.second] > 0);
}
char MapData::checkisChange(std::pair<int, int> pos) {
	if (nxtMap[pos.first][pos.second] != nowMap[pos.first][pos.second]) {
		return nxtMap[pos.first][pos.second];
	}
	else {
		return 'N';
	}
}