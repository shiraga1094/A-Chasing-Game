#ifndef _MapData_
#define _MapData_

#include <iostream>
#include <vector>
#include "StageModeData.h"
const int Width = 21, Height = 21;

class MapData {
private:
	char nowMap[Width][Height];
	char nxtMap[Width][Height];
	int DangerMap[Width][Height];
	int Obstacle[Width][Height];
public:
	MapData();
	void inputObstacle(int MapNo, StageModeData* data);
	void Clear();
	void Initialize();
	std::pair<int, int> giveLegalPos(std::vector<int> border, std::pair<int, int> tmp = { -1, -1 });
	char checknowMap(std::pair<int, int> pos);
	bool checkObstacle(std::pair<int, int> pos);
	char checknxtMap(std::pair<int, int> pos);
	void Update_DangerMap(std::pair<int, int> pos={-1, -1});
	void Update_nxtMap(std::pair<int, int> nowPos, char ch);
	void Update_ObstacleMap(std::pair<int, int> pos, bool isSet);
	bool isInDanger(std::pair<int, int> pos);
	char checkisChange(std::pair<int, int> pos); //If no output 'N'
};






#endif
