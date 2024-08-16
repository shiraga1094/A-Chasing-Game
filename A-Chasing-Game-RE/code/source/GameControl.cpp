#include "GameControl.h"

GameControl::GameControl() {
	character = new Character();
	mapdata = new MapData();
	gamedata = new GameData();
	enemylist = new Enemylist();
	item = new Item(mapdata);
	score = new Score(mapdata);
	status = Open;
	view = new DrawWindow(mapdata);
	stagemodedata = new StageModeData();
	message = new Message(&AbilityNo, &GameModeNo);
	achievement = new Achievement(&AbilityNo, &GameModeNo);
	ability = new Ability(mapdata, gamedata, character, enemylist, 
		view, item, score, message);
	opening = new Opening(gamedata, view, stagemodedata, 
		achievement, &MapNo, &AbilityNo, &GameModeNo);
}
void GameControl::OpeningSet() {
	stagemodedata->WriteIn();
	achievement->Input();
	opening->Run();
	mapdata->inputObstacle(MapNo, stagemodedata);
	int level = AbilityNo / 10, now = AbilityNo % 10;
	if (GameModeNo != 3) {
		message->inputName(stagemodedata->giveAbility_Name(level, now),
			stagemodedata->giveAbilityIntro(level, now));
	}
	status = Initial;
}
void GameControl::Run() {
	srand(time(NULL));
	while (status != Exit) {
		if (status == Initial) {
			Initialize();
		}
		else if (status == Open)
			OpeningSet();
		else if (status == Start) {
			handleGameEvent();
		}
		else if (status == GameOver) {
			GameOverEvent();
		}
	}
}
InputStatus GameControl::giveInput() {
	char key = _getch();
	switch (key) {
		case ' ': return Space;
		case 119: return Up;
		case 97: return Left;
		case 100: return Right;
		case 115: return Down;
		default: return giveInput();
	}
}
int GameControl::giveRandom(int X) {
	return std::rand() % (X + 1);
}
bool GameControl::isLegalMove(std::pair<int, int> pos, InputStatus key) {
	switch (key) {
		case Up: pos.second-=character->giveTeleport_Range(); break;
		case Right: pos.first+=character->giveTeleport_Range(); break;
		case Down: pos.second+=character->giveTeleport_Range(); break;
		case Left: pos.first-=character->giveTeleport_Range(); break;
	}
	if (pos.first < 0 || pos.first >= Width || pos.second < 0 || pos.second >= Height) {
		return false;
	}
	if (mapdata->checkObstacle(pos)) {
		return false;
	}
	return true;
}
void GameControl::handleGameEvent() {
	RoundInitailize();
	EnemySpawnEvent();
	ItemSpawnEvent();
	EnemyMoveEvent();
	CheckBeforePassiveEvent();
    
	CheckInputEvent(); // => CharacterMove or UseAbility

	CheckItemEvent();
	CheckScoreEvent();
	CheckPassiveAbilityEvent();
	CheckEnemyDead();
	DrawWindow_Change();
	CheckCharacterDead();
}
void GameControl::Initialize() {
	DrawWindow_Initial();
	mapdata->Clear();
	character->setPos(gamedata->Character_Initial_Pos);
	mapdata->Update_nxtMap(character->givePos(), 'C');
	enemylist->clear();
	for (int i=0; i<gamedata->EnemyAmount; i++){
		std::pair<int, int> tmpPos = gamedata->Enemy_Initial_Pos[i];
		int tmpType = gamedata->Enemy_Initial_Type[i];
		enemylist->push_back(tmpPos, tmpType, mapdata);
		mapdata->Update_nxtMap(tmpPos, 'E');
	}
	enemylist->cleartarget();
	ability->Initial(&AbilityNo);
	view->DrawChange();
	view->DrawObstacle();
	gamedata->Enemy_count = gamedata->EnemyAmount;
	gamedata->Item_count = 2;
	gamedata->Life_count = 3;
	gamedata->Kill_count = gamedata->Score_count = 0;
	gamedata->Max_Item_count = gamedata->Max_Score_count = 0;
	gamedata->Round_count = 0;
	item->ResetPos();
	gamedata->isGetItem = gamedata->isGetScore = false;
	message->outputName();
	status = Start;
}
void GameControl::RoundInitailize() {
	enemylist->inputtarget(character->givePos());
	view->DrawStatus(gamedata);
	mapdata->Initialize();
}
void GameControl::EnemySpawnEvent() {
	if (giveRandom(gamedata->EnemyRandomLimit[GameModeNo])==0 || 
		enemylist->size()<gamedata->EnemyAmountLimit[GameModeNo]) {
		std::pair<int, int> tmpPos = mapdata->giveLegalPos({0, 0, Width, Height}, character->givePos());
		if (gamedata->Enemy_count < gamedata->EnemyTypeLimit[GameModeNo]) {
			enemylist->push_back(tmpPos, giveRandom(2), mapdata);
		}
		else {
			enemylist->push_back(tmpPos, giveRandom(4), mapdata);
		}
		gamedata->Enemy_count++;
	}
}
void GameControl::ItemSpawnEvent() {
	if (giveRandom(gamedata->ItemRandomLimit[GameModeNo])==0) {
		if(item->giveAmount()<2)
			item->setPos(-1, mapdata->giveLegalPos({0,0,Width,Height}));
	}
	if (giveRandom(gamedata->ItemRandomLimit[GameModeNo]) == 0) {
		if(score->giveAmount()<gamedata->ItemAmountLimit[GameModeNo])
			score->setPos(-1, mapdata->giveLegalPos({ 0,0,Width,Height }));
	}
}
void GameControl::EnemyMoveEvent() {
	mapdata->Update_DangerMap();
	for (int i = 0; i < enemylist->size(); i++) {
		enemylist->setScatter(i, gamedata->ScatterRandom[GameModeNo]);
		std::pair<int, int> target = enemylist->givetarget();
		enemylist->Move(i, target);
	}
}
void GameControl::CheckBeforePassiveEvent() {
	enemylist->cleartarget();
	ability->Before_Passive();
	achievement->CheckSpecialUnlock(gamedata);
}
void GameControl::CheckInputEvent() {
	while (true) {
		InputStatus key = giveInput();
		if (key == Space) {
			UseAbilityEvent();
		}
		else {
			if (CharacterMoveEvent(key))
				break;
		}
	}
}
bool GameControl::CharacterMoveEvent(InputStatus key) {
	if (!isLegalMove(character->givePos(), key))
		return false;
	if (AbilityNo == 33) {
		std::pair<int, int> tmp = character->givePos();
		switch (key) {
		case Up: tmp.second -= character->giveTeleport_Range(); break;
		case Right: tmp.first += character->giveTeleport_Range(); break;
		case Down: tmp.second += character->giveTeleport_Range(); break;
		case Left: tmp.first -= character->giveTeleport_Range(); break;
		}
		character->AI_Move(score, mapdata);
		if (character->givePos() != tmp) {
			message->input("But it refused.");
		}
	}
	else {
		character->Move(key);
		gamedata->lastkey = key;
	}
	mapdata->Update_nxtMap(character->givePos(), 'C');
	gamedata->Round_count++;
	return true;
}
void GameControl::UseAbilityEvent() {
	ability->Active();
}
void GameControl::CheckPassiveAbilityEvent() {
	ability->Passive();
}
void GameControl::CheckItemEvent() {
	if (item->isGetItem(character->givePos())) {
		gamedata->Item_count = min(5, gamedata->Item_count + 1);
		gamedata->isGetItem = true;
		gamedata->isPerfect = false;
		gamedata->Max_Item_count++;
	}
	else {
		gamedata->isGetItem = false;
	}
}
void GameControl::CheckScoreEvent() {
	if (score->isGetScore(character->givePos())) {
		gamedata->Score_count++;
		gamedata->isGetScore = true;
		gamedata->isPerfect = false;
		gamedata->Max_Score_count++;
	}
	else {
		gamedata->isGetScore = false;
	}
}
void GameControl::CheckEnemyDead() {
	gamedata->last_Kill_count = gamedata->Kill_count;
	for (int i = 0; i < enemylist->size(); i++) {
		if (enemylist->checkisDead(i)) {
			view->SetEffect(enemylist->givePos(i), 'E', 252);
			if (!enemylist->isFiction(i)) {
				gamedata->Enemy_count--;
				gamedata->Kill_count++;
			}
			enemylist->erase(i);
			i--;
			
		}
	}
}
void GameControl::CheckCharacterDead() {
	if (mapdata->isInDanger(character->givePos())) {
		character->toDead();
		gamedata->isPerfect = false;
	}
	if (character->checkisDead()) {
		gamedata->Life_count--;
		message->input("You only live once.");
		view->SetQuickEffect(character->givePos(), 'C', 4);
		for (int i = 0; i < enemylist->size(); i++) {
			enemylist->setParalyze(i, 1);
		}
		if (gamedata->Life_count <= 0) {
			status = GameOver;
			view->SetQuickEffect(character->givePos(), 'X', -1);
		}
	}
}
void GameControl::GameOverEvent() {
	message->input("You Died.");
	message->output();
	view->DrawStatus(gamedata);
	Sleep(3000);
	view->DrawGameOver();
	Sleep(3000);
	achievement->Update(gamedata);
	achievement->Output();
	view->DrawAchievement(achievement, &GameModeNo);
	_getch();
	status = Exit;
}
void GameControl::DrawWindow_Initial() {
	view->DrawMap();
	view->DrawStatusFrame();
}
void GameControl::DrawWindow_Change() {
	view->DrawChange();
	view->DrawObstacle();
	view->DrawStatus(gamedata);
	message->output();
}