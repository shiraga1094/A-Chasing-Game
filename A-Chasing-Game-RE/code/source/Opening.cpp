#include "Opening.h"

Opening::Opening(GameData* _gamedata, DrawWindow* _view, StageModeData* _data, int* _MapNo, int* _AbilityNo, int* _GameModeNo) {
	gamedata = _gamedata;
	view = _view;
	MapNo = _MapNo;
	stagemodedata = _data;
	AbilityNo = _AbilityNo;
	GameModeNo = _GameModeNo;
}
void Opening::AllClear() {
	for (int x = 0; x < 60; x++) {
		for (int y = 0; y < 60; y++) {
			gotoxy(x, y); std::cout << ' ';
		}
	}
}
void Opening::Run() {
	Welcome();
	Tip();
	GameModeChoise();
	MapChoise();
	if (*GameModeNo != 3) {
		AbilityChoise();
	}
	else {
		*AbilityNo = -1;
	}
}
void Opening::Welcome() {
	int x = 17, y = 10;
	gotoxy(x, y); std::cout << "A chasing game.";
	gotoxy(x - 4, y + 1); std::cout << "Press any key to continue.";
	_getch();
	AllClear();
}
void Opening::Tip() {
	int x = 24, y = 10;
	gotoxy(x, y); std::cout << "How to play:";
	gotoxy(x - 3, y + 1); std::cout << "Use WASD to move.";
	gotoxy(x - 7, y + 2); std::cout << "Press space to use ability.";
	gotoxy(x - 16, y + 3); std::cout << "'C'=Character, 'I'=Item, 'E'=Enemy, 'S'=Score";
	gotoxy(x - 10, y + 4); std::cout << "Game ends when your life is zero.";
	gotoxy(x + 1, y + 5); std::cout << "Have fun.";
	gotoxy(x - 7, y + 6); std::cout << "(Press any key to start.)";
	_getch();
	AllClear();
}
void Opening::GameModeChoise() {
	int x = 17, y = 10;
	gotoxy(x, y); std::cout << "Select Gamemode:";
	gotoxy(x + 4, y + 1); std::cout << "Easy";
	gotoxy(x + 4, y + 2); std::cout << "Medium";
	gotoxy(x + 4, y + 3); std::cout << "Hard";
	gotoxy(x - 3, y + 6); std::cout << "(Press space to start.)";

	int pos = 0; x = 19; y = 11;
	while (true) {
		gotoxy(x, y + pos);
		SetColor(11); std::cout << 'O'; SetColor();
		char key = _getch();
		gotoxy(x, y + pos); std::cout << ' ';
		if (key == 's') {
			pos++;
			if (pos > 2) pos = 0;
		}
		else if (key == 'w') {
			pos--;
			if (pos < 0) pos = 2;
		}
		else if (key == ' ') {
			*GameModeNo = pos;
			AllClear();
			return;
		}
		
	}
}
void Opening::MapChoise() {
	int x = 17, y = 10;
	gotoxy(x, y); std::cout << "Choose Your Map.";
	gotoxy(x + 3, y+1); std::cout << "MapNo: ";
	int pos = 0; x = 26; y = 11;
	while (true) {
		gotoxy(x, y); std::cout << pos + 1;
		char key = _getch();
		gotoxy(x, y); std::cout << "   ";
		if (key == ' ') {
			*MapNo = pos;
			AllClear();
			return;
		}
		else if (key == 'd') {
			pos++; if (pos >= 100) pos = 0;
		}
		else if (key == 'a') {
			pos--; if (pos < 0) pos = 99;
		}
	}
}
void Opening::AbilityChoise() {
	int x = 15, y = 10;
	gotoxy(x, y); std::cout << "Choose Your Ability:";
	gotoxy(x + 3, y + 1); std::cout << "Level: ";
	gotoxy(x + 3, y + 2); std::cout << "Name: ";

	int colorno[5] = { 10,12,3,4,8 };
	x = 16; y = 11;
	int now = 0, level = 0, pos = 0;
	while (true) {
		gotoxy(x, y + now);
		SetColor(11); std::cout << 'O'; SetColor();
		gotoxy(24, 11); SetColor(colorno[level]); std::cout << level +1;
		gotoxy(23, 12); std::cout << stagemodedata->giveAbility_Name(level, pos);
		SetColor();
		char key = _getch();
		gotoxy(x, y + now); std::cout << ' ';
		gotoxy(23, 12); std::cout << "                                         ";
		if (key == ' ') {
			AllClear();
			*AbilityNo = level * 10 + pos;
			return;
		}
		else if (key == 'w') {
			now--; if (now < 0) now = 1;
			pos = 0;
		}
		else if (key == 's') {
			now++; if (now > 1) now = 0;
			pos = 0;
		}
		else if (key == 'd') {
			if (now == 0) {
				level++; if (level >= stagemodedata->giveAbilityLevelLimit()) level = 0;
			}
			else {
				pos++; if (pos >= stagemodedata->giveAbilityAmount(level)) pos = 0;
			}
		}
		else if (key == 'a') {
			if (now == 0) {
				level--; if (level < 0) level = stagemodedata->giveAbilityLevelLimit() - 1;
			}
			else {
				pos--; if (pos < 0) pos = stagemodedata->giveAbilityAmount(level) - 1;
			}
		}
	}
}