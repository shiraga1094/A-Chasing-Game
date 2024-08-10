#ifndef _Opening_
#define _Opening_

#include <iostream>
#include "GameData.h"
#include "DrawWindow.h"
#include "StageModeData.h"
#include "CursorControl.h"

class Opening {
private:
	GameData* gamedata;
	DrawWindow* view;
	StageModeData* stagemodedata;
	int *MapNo;
	int *AbilityNo;
	int *GameModeNo;
	void AllClear();
public:
	Opening(GameData* _gamedata, DrawWindow* _view, StageModeData* _data, int* _MapNo, int* _AbilityNo, int* _GameModeNo);
	void Run();
	void Welcome();
	void Tip();
	void GameModeChoise();
	void MapChoise();
	void AbilityChoise();
};




#endif
