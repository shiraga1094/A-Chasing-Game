#ifndef _DrawWindow_
#define _DrawWindow_

#include <iostream>
#include "CursorControl.h"
#include "MapData.h"
#include "GameData.h"

const int MapBx = 0, MapBy = 3;
class DrawWindow {
private:
	MapData* mapdata;
	int ColorMap[Width][Height];
	char EffectMap[Width][Height];
	bool ClearMap[Width][Height];
	

public:
	DrawWindow(MapData* _mapdata);
	void WindowClear();
	void OnColor(char ch, int color=-1);
	void DrawMap();
	void DrawChange();
	void DrawStatusFrame();
	void DrawStatus(GameData* gamedata);
	void DrawObstacle();
	void SetQuickEffect(std::pair<int, int> pos, char ch, int color);
	void SetEffect(std::pair<int, int> pos, char ch, int color);
	void SetClear(std::pair<int, int> pos);
	void DrawGameOver();

};





#endif

