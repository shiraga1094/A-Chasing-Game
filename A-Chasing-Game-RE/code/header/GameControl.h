#include <iostream>
#include <deque>
#include <vector>

#include "Character.h"
#include "Enemy.h"
#include "Enemylist.h"
#include "Item.h"
#include "Score.h"
#include "Ability.h"
#include "MapData.h"
#include "Opening.h"
#include "DrawWindow.h"
#include "GameData.h"
#include "EnumStatus.h"
#include "StageModeData.h"
#include "Message.h"


class GameControl {
private:
	Character* character;
	Enemylist* enemylist;
	Item* item;
	Score* score;
	Ability* ability;
	Opening* opening;
	MapData* mapdata;
	GameStatus status;
	GameData* gamedata;
	StageModeData* stagemodedata;
	DrawWindow* view;
	Message* message;

	int MapNo;
	int AbilityNo;
	int GameModeNo;

public:
	//Initial
	GameControl();
	void Initialize();
	void OpeningSet();
	void Run();
	
	//Tool
	InputStatus giveInput();
	int giveRandom(int X);
	bool isLegalMove(std::pair<int, int> pos, InputStatus key);

	//Handler
	void handleGameEvent();
	
	//Event
	void RoundInitailize();
	void EnemySpawnEvent();
	void ItemSpawnEvent();
	void EnemyMoveEvent();
	void CheckBeforePassiveEvent();
	void CheckInputEvent();
	bool CharacterMoveEvent(InputStatus key);
	void UseAbilityEvent();
	void CheckPassiveAbilityEvent();
	void CheckItemEvent();
	void CheckScoreEvent();
	void CheckEnemyDead();
	void CheckCharacterDead();
	void GameOverEvent();
	
	//Draw
	void DrawWindow_Initial();
	void DrawWindow_Change();

};
