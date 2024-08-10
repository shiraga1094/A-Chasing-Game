#include "DrawWindow.h"

DrawWindow::DrawWindow(MapData* _mapdata) {
	mapdata = _mapdata;
	HideCursor();
	for (int r = 0; r < Width; r++) {
		for (int c = 0; c < Height; c++) {
			ColorMap[r][c] = 0;
			EffectMap[r][c] = ' ';
			ClearMap[r][c] = false;
		}
	}
}
void DrawWindow::WindowClear() {
	for (int x = 0; x < 80; x++) {
		for (int y = 0; y < 26; y++) {
			gotoxy(x, y);
			std::cout << ' ';
		}
	}
}
void DrawWindow::OnColor(char ch, int color) {
	if (color == -1) {
		switch (ch) {
			case 'C': SetColor(11); break;
			case 'E': SetColor(12); break;
			case 'I': SetColor(10); break;
			case 'O': SetColor(13); break;
			case 'S': SetColor(14); break;
			case 'P': SetColor(2);  break;
			case 'W': SetColor(136); break;
		}
	}
	else {
		SetColor(color);
	}
	std::cout << ch;
	SetColor();
}
void DrawWindow::DrawMap() {
	//Draw Frame
	for (int x = MapBx; x <= MapBx+(Width*2-1) + 1; x++) {
		gotoxy(x, MapBy); std::cout << '-';
		gotoxy(x, MapBy + Height + 1); std::cout << '-';
	}
	for (int y = MapBy + 1; y <= MapBy + Height; y++) {
		gotoxy(MapBx, y); std::cout << '|';
		gotoxy(MapBx + (Width * 2 - 1) + 1, y); std::cout << '|';
	}

	//Draw Map
	for (int dx = 0; dx < Width; dx++) {
		for (int dy = 0; dy < Height; dy++) {
			gotoxy(MapBx + dx * 2 + 1, MapBy + dy + 1);
			OnColor(mapdata->checknowMap({dx, dy}));
		}
	}
}
void DrawWindow::DrawChange() {
	for (int dx = 0; dx < Width; dx++) {
		for (int dy = 0; dy < Height; dy++) {
			char now = mapdata->checkisChange({ dx,dy });
			if (ClearMap[dx][dy]) {
				ClearMap[dx][dy] = false;
				gotoxy(MapBx + dx * 2 + 1, MapBy + dy + 1);
				std::cout << '.';
			}
			if (EffectMap[dx][dy] != ' ') {
				gotoxy(MapBx + dx * 2 + 1, MapBy + dy + 1);
				if (ColorMap[dx][dy] != 0) {
					OnColor(EffectMap[dx][dy], ColorMap[dx][dy]);
					ColorMap[dx][dy] = 0;
				}
				else {
					OnColor(EffectMap[dx][dy]);
				}
				EffectMap[dx][dy] = ' ';
			}
			else if (now!='N') {
				gotoxy(MapBx + dx * 2 + 1, MapBy + dy + 1);
				if (ColorMap[dx][dy] != 0) {
					OnColor(now, ColorMap[dx][dy]);
					ColorMap[dx][dy] = 0;
				}
				else {
					OnColor(now);
				}
			}
		}
	}
}
void DrawWindow::DrawStatusFrame() {
	gotoxy(1, 1); std::cout << "SKILL:";
	gotoxy(13, 1); std::cout << "ENEMY:";
	gotoxy(22, 1); std::cout << "SCORE:";
	gotoxy(31, 1); std::cout << "KILL:";
	gotoxy(39, 1); std::cout << "LIFE:";
}
void DrawWindow::DrawStatus(GameData* gamedata) {
	gotoxy(7, 1); std::cout << "      ";
	for (int i = 0; i < gamedata->Item_count; i++) {
		gotoxy(7 + i, 1); OnColor(' ', 119);
	}
	gotoxy(19, 1); std::cout << "  "; 
	gotoxy(19, 1); std::cout << gamedata->Enemy_count;
	gotoxy(28, 1); std::cout << "  "; 
	gotoxy(28, 1); std::cout << gamedata->Score_count;
	gotoxy(36, 1); std::cout << "  "; 
	gotoxy(36, 1); std::cout << gamedata->Kill_count;
	gotoxy(44, 1); std::cout << ' ';
	gotoxy(44, 1); std::cout << gamedata->Life_count;
}
void DrawWindow::DrawObstacle() {
	for (int dx = 0; dx < Width; dx++) {
		for (int dy = 0; dy < Height; dy++) {
			gotoxy(MapBx + dx * 2 + 1, MapBy + dy + 1);
			if (mapdata->checkObstacle({ dx, dy })) {
				OnColor('.', 136);
			}
		}
	}
	for (int dx = 0; dx < Width-1; dx++) {
		for (int dy = 0; dy < Height; dy++) {
			gotoxy(MapBx + dx * 2 + 2, MapBy + dy + 1);
			if (mapdata->checkObstacle({ dx, dy }) && mapdata->checkObstacle({dx+1, dy})){
				OnColor('.', 136);
			}
		}
	}
}
void DrawWindow::SetQuickEffect(std::pair<int, int> pos, char ch, int color) {
	gotoxy(MapBx + pos.first * 2 + 1, MapBy + pos.second + 1);
	OnColor(ch, color);
}
void DrawWindow::SetEffect(std::pair<int, int> pos, char ch, int color) {
	EffectMap[pos.first][pos.second] = ch;
	ColorMap[pos.first][pos.second] = color;
}
void DrawWindow::SetClear(std::pair<int, int> pos) {
	ClearMap[pos.first][pos.second] = true;
}
void DrawWindow::DrawGameOver() {
	WindowClear();
	gotoxy(20, 10); std::cout << "Game Over";
}