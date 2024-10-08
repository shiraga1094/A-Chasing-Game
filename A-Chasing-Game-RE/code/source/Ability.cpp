#include "Ability.h"

Ability::Ability(MapData* _mapdata, GameData* _gamedata, Character* _ch, 
	Enemylist* _enemy, DrawWindow* _view, Item* _item, Score* _score, Message* _message) {
	mapdata = _mapdata;
	gamedata = _gamedata;
	character = _ch;
	enemylist = _enemy;
	item = _item;
	score = _score;
	view = _view;
	message = _message;
}
void Ability::Initial(int *_abilityno) {
	AbilityNo = _abilityno;
}
void Ability::Active() {
	std::pair<int, int> ch = character->givePos();
	switch (*AbilityNo) {
		case 0:
			if (Effect[0] >= 2) return;
			if (gamedata->Item_count <= 0) return;
			gamedata->Item_count--; Effect[0]++;
			grid_0[ch.first][ch.second] = Effect[0];
			break;
		case 1:
			if (Effect[1] == 1) return;
			if (gamedata->Item_count < 3) return;
			gamedata->Item_count-=3; Effect[1] = 1;
			character->setUnDead();
			DrawEffect();
			break;
		case 2:
			if (gamedata->Item_count <= 0) return;
			gamedata->Item_count--;
			character->addTeleport_Range();
			break;
		case 3:
			if (Effect[3] == 1) return;
			if (gamedata->Item_count <= 0) return;
			gamedata->Item_count--; Effect[3] = 1;
			DrawEffect();
			break;
		case 4:
			if (Effect[4] >= 2) return;
			if (Effect[4] == 0 && gamedata->Item_count < 2) return;
			if (Effect[4] == 1 && gamedata->Item_count < 3) return;
			if (Effect[4] == 0) {
				gamedata->Item_count -= 2; Effect[4] = 1;
			}
			else {
				gamedata->Item_count = 0; Effect[4] = 2;
			}
			break;
		case 10:
			if (Effect[10] > 0) return;
			if (gamedata->Item_count < 3) return;
			gamedata->Item_count -= 3;
			Effect[10] += 20; Count[10] += 20;
			for (int i = 0; i < enemylist->size(); i++) {
				enemylist->setCountDown(i, 20);
			}
			item->ResetPos();
			break;
		case 12:
			if (gamedata->Item_count < 3) return;
			gamedata->Item_count -= 3;
			Effect[12] = 1;
			for (int i = 0; i < 5; i++) {
				enemylist->push_back(mapdata->giveLegalPos({ ch.first - 3, ch.second - 3, ch.first + 4, ch.second + 4 }), 5, mapdata);
				enemylist->setCountDown(enemylist->size() - 1, 21);
				enemylist->setFiction(enemylist->size() - 1);
			}
			break;
		case 13:
			if (Effect[13] == 0) {
				Effect[13] = 1; Count[13] = 30;
				return;
			}
			if (Effect[13] == 1) {
				if (gamedata->Item_count < 5) return;
				gamedata->Item_count -= 5;
				Effect[13] = 0; Count[13] = 0;
			}
			break;
		case 20:
			if (gamedata->Item_count <= 0) return;
			gamedata->Item_count--;
			for (int i = 0; i < enemylist->size(); i++) {
				enemylist->toDead(i);
			}
			enemylist->push_back(mapdata->giveLegalPos({ 0,0,Width,Height }, ch), std::rand() % 5, mapdata);
			enemylist->setUnDead(enemylist->size() - 1);
			gamedata->Enemy_count++;
			break;
		case 23:
			if (Effect[23] == 1) return;
			if (gamedata->Item_count < 5) return;
			gamedata->Item_count -= 5;
			Effect[23] = 1;
			item->ResetPos();
			break;
		case 32:
			if (gamedata->Item_count < 3) return;
			gamedata->Item_count-=3;
			message->input(str_32[Effect[32]]);
			message->output();
			break;
		case 42:
			if (Effect[42] == 1) return;
			if (gamedata->Item_count < 2) return;
			gamedata->Item_count -= 2;
			Effect[42] = 1;
			break;
	}
	view->DrawStatus(gamedata);
}

void Ability::Before_Passive() {
	std::pair<int, int> ch = character->givePos();
	switch (*AbilityNo) {
		case 11:
			DrawEffect();
			break;
		case 20:
			for (int i = 0; i < enemylist->size(); i++) {
				if (enemylist->checkisUnDead(i)) {
					view->SetEffect(enemylist->givePos(i), 'E', 13);
				}
			}
			break;
		case 21:
			gamedata->Item_count = 0;
			item->deleteItem();
			Q_21.clear();
			for (int i = 0; i < enemylist->size(); i++) {
				if (enemylist->givehistoryPos(i).first != -1) {
					if (enemylist->givehistoryPos(i) == ch) {
						enemylist->toDead(i);
					}
					else {
						Q_21.push_back(enemylist->givehistoryPos(i));
					}
				}
			}
			DrawEffect();
			break;
		case 22:
			{
			std::pair<int, int> tmp;
			for (int i = 0; i < enemylist->size(); i++) {
				tmp = enemylist->givePos(i);
				for (int s = 0; s < 3; s++) {
					if (enemylist->givePos(i) == ch) {
						view->SetQuickEffect(enemylist->givePos(i), 'C', 75);
					}
					else {
						view->SetQuickEffect(enemylist->givePos(i), 'E', 8);
					}
					enemylist->Move(i, ch);
					view->SetEffect(enemylist->givePos(i), '.', 7);
					mapdata->Update_DangerMap(enemylist->givePos(i));
				}
				enemylist->setPos(i, tmp);
			}
			break;
			}
		case 30:
			item->deleteItem();
			for (int i = 0; i < enemylist->size(); i++) {
				enemylist->SetEnemyType(i, Straight);
				enemylist->setCaptureScore(i);
			}

			break;
		case 31:
			for (int i = 0; i < enemylist->size(); i++) {
				enemylist->SetEnemyType(i, Straight);
				enemylist->setCaptureItem(i);
			}
			break;
		case 40:
			view->SetClear(knife);
			break;
		case 41:
			for (int i = 0; i < enemylist->size(); i++) {
				enemylist->setCaptureItem(i);
				enemylist->setCaptureScore(i);
			}
			break;
		case 42:
			view->SetClear(knife);
			for (std::pair<int, int> tmp : enemyknife) {
				view->SetClear(tmp);
			}
			enemyknife.clear();
			break;
		case 43:
			if (Count[43] < 1000) {
				view->SetQuickEffect(ch, 'C', 11);
			}
			break;
		case 47:
			if (Count[47] <= 100) {
				message->input(str_47[0]);
			}
			else if (Count[47] <= 200) {
				message->input(str_47[1]);
			}
			else if (Count[47] <= 300) {
				message->input(str_47[2]);
			}
			message->output();
			break;
		case 50:
			if (gamedata->Round_count == 0) {
				standard = clock();
			}
			break;
	}
}

void Ability::Passive() {
	std::pair<int, int> ch = character->givePos();
	switch (*AbilityNo) {
		case 0:
		{
		Effect[0] = 0;
		if (grid_0[ch.first][ch.second] > 0) {
			character->toDead();
			if (grid_0[ch.first][ch.second] == 2)
				Q_0.push_back({ ch.first, ch.second });
			grid_0[ch.first][ch.second] = 0;
		}
		for (int i = 0; i < enemylist->size(); i++) {
			std::pair<int, int> pos = enemylist->givePos(i);
			if (grid_0[pos.first][pos.second] > 0) {
				enemylist->toDead(i);
				if (grid_0[pos.first][pos.second] == 2)
					Q_0.push_back({ pos.first, pos.second });
				grid_0[pos.first][pos.second] = 0;
			}
		}
		int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
		int dy[8] = { 1,0,-1,1,-1,1,0,-1 };
		std::pair<int, int> tmp;
		while (!Q_0.empty()) {
			std::pair<int, int> now = Q_0.front(); Q_0.pop_front();
			grid_0[now.first][now.second] = 0;
			for (int i = 0; i < 8; i++) {
				tmp = { now.first + dx[i], now.second + dy[i] };
				if (tmp.first < 0 || tmp.first >= Width || tmp.second < 0 || tmp.second >= Height) {
					continue;
				}
				if (ch == tmp) character->toDead();
				for (int i = 0; i < enemylist->size(); i++) {
					if (enemylist->givePos(i) == tmp) {
						enemylist->toDead(i);
					}
				}
				if (grid_0[tmp.first][tmp.second] > 0) {
					if (grid_0[tmp.first][tmp.second] == 2) Q_0.push_back(tmp);
					grid_0[tmp.first][tmp.second] = 0;
					view->SetQuickEffect({ tmp.first, tmp.second }, '.', 7);
				}
			}
		}
		break;
		}
		case 1:
			Effect[1] = 0;
			break;
		case 3:
			Effect[3] = 0;
			break;
		case 4:
			if (Effect[4] == 1) Count[4] = 2;
			else if(Effect[4]==2) Count[4] = 5;
			for (int i = 0; i < enemylist->size(); i++) {
				if (Effect[4] == 1) enemylist->setParalyze(i, 2);
				else if (Effect[4] == 2) enemylist->setParalyze(i, 5);
			}
			Effect[4] = 0;
			break;
		case 10:
			if (Effect[10] > 0) {
				Effect[10]--;
				if (Count[10] > 0) {
					Count[10]--;
					if (Count[10] == 0)
						character->toDead();
				}
				if (gamedata->isGetItem)
					Count[10] = 0;
			}
			break;
		case 11:
			if (Effect[11] == 0 && gamedata->Item_count == 5) {
				gamedata->Item_count = 0; Effect[11] = 1;
			}
			if (Effect[11] == 1 && gamedata->Item_count == 3) {
				gamedata->Item_count = 0; Effect[11] = 0;
			}
			return;
		case 12:
			for (int i = 0; i < enemylist->size(); i++) {
				if (enemylist->isFiction(i)) {
					enemylist->inputtarget(enemylist->givePos(i));
				}
			}
			Effect[12] = 0;
			break;
		case 13:
			if (Effect[13] == 1) {
				Count[13]--;
				if (Count[13] == 0) {
					Effect[13] = 0;
					character->toDead();
				}
				character->setUnDead();
			}
			break;
		case 14:
			gamedata->Item_count = 0;
			Count[14]++; if (Count[14] == 3) Count[14] = 0;
			break;
		case 15:
			if (Effect[15] == 0) Effect[15] = 50;
			if (Effect[15] > 0) {
				Effect[15]--;
				if (mapdata->isInDanger(character->givePos())) {
					Count[15] = 0;
					character->setUnDead();
					gamedata->Item_count = max(gamedata->Item_count - 2, 0);
				}
				if (Effect[15] == 0) {
					Effect[15] = 50;
					if (Count[15] == 0 && gamedata->Item_count < 3) {
						character->checkisDead();
						character->toDead();
					}
					else if (Count[15] == 0) {
						gamedata->Item_count -= 3;
					}
					Count[15] = 1;
				}
			}
			break;
		case 21:
			gamedata->Item_count = 0;
			for (int i = 0; i < enemylist->size(); i++) {
				if (enemylist->givehistoryPos(i)==ch) {
					enemylist->toDead(i);
				}
			}
			for (int i = 0; i < enemylist->size(); i++) {
				if (enemylist->checkisDead(i)) {
					enemylist->setParalyze(i, 30);
					gamedata->Kill_count++;
				}
				if (enemylist->checkisParalyze(i)) {
					enemylist->ResetHistory(i);
				}
			}
			break;
		case 22:
			for (int i = 0; i < enemylist->size(); i++) {
				view->SetEffect(enemylist->givePos(i), '.', 7);
			}
			view->SetEffect(ch, 'C', 11);
			return;
		case 23:
			if (Effect[23] == 1) {
				if (gamedata->isGetItem) {
					Effect[23] = 0;
					item->ResetPos();
				}
				else {
					character->setUnDead();
					score->deleteScore();
					Count[23]++;
					if (Count[23] == 5) {
						Count[23] = 0;
						item->ResetPos();
					}
				}
			}
			break;
		case 24:
			item->deleteItem();
			if (gamedata->isGetScore) {
				character->setPos(mapdata->giveLegalPos({ 0,0,Width,Height }));
				for (int i = 0; i < enemylist->size(); i++) {
					enemylist->setPos(i, mapdata->giveLegalPos({ 0,0,Width,Height }), true);
				}
			}
			break;
		case 30:
			item->deleteItem();
			for (int i = 0; i < enemylist->size(); i++) {
				if (score->isGetScore(enemylist->givePos(i))) {
					view->SetEffect(enemylist->givePos(i), 'E', 12);
					gamedata->Enemy_getItem_count++;
				}
				if (enemylist->givePos(i) == ch) {
					enemylist->toDead(i);
				}
			}
			enemylist->cleartarget();
			for (int i = 0; i < 3; i++) {
				for (std::pair<int, int> tmp : score->givePos()) {
					enemylist->inputtarget(tmp);
				}
			}
			break;
		case 31:
			if (Count[31] == 0) {
				Count[31] = 50; gamedata->Item_count = 0; Cnt_31 = { 0,0 };
			}
			if (gamedata->isGetItem) {
				Cnt_31.second++;
				gamedata->Item_count--;
			}
			for (int i = 0; i < enemylist->size(); i++) {
				if (item->isGetItem(enemylist->givePos(i))) {
					view->SetEffect(enemylist->givePos(i), 'E', 12);
					Cnt_31.first++;
				}
			}
			if (Count[31] > 0) {
				Count[31]--;
				if (Count[31] == 0) {
					Count[31] = 50;
					gamedata->Item_count = 0;
					if (Cnt_31.first > Cnt_31.second) {
						character->toDead();
					}
					else if (Cnt_31.first < Cnt_31.second) {
						for (int i = 0; i < enemylist->size(); i++) {
							enemylist->toDead(i);
						}
					}
					else {
						character->toDead();
						for (int i = 0; i < enemylist->size(); i++) {
							enemylist->toDead(i);
						}
					}
					Cnt_31 = { 0,0 };
				}
				else if (Count[31] % 10 == 0) gamedata->Item_count++;
			}
			for (int i = 0; i < 4; i++) {
				for (std::pair<int, int> tmp : item->givePos()) {
					enemylist->inputtarget(tmp);
				}
			}
			break;
		case 32:
			{
			if (Count[32] == 0) {
				Count[32] = 50; Effect[32] = std::rand() % 3;
			}
			if (Count[32] > 0) {
				Count[32]--;
				if (Count[32] == 0) {
					Count[32] = 0;
					if (Effect[32] == 0) {
						int tmp;
						for (int i = 0; i < enemylist->size(); i++) {
							tmp = 0;
							std::pair<int, int> pos = enemylist->givePos(i);
							for (int r = 0; r < Width; r++) {
								if (mapdata->checknxtMap({ r, pos.second })=='E') {
									tmp++;
								}
								if (ch.first == r && ch.second == pos.second) {
									tmp++;
								}
							}
							for (int c = 0; c < Height; c++) {
								if (mapdata->checknxtMap({ pos.first, c}) == 'E') {
									tmp++;
								}
								if (ch.first == pos.first && ch.second == c) {
									tmp++;
								}
							}
							if (tmp > 2) enemylist->toDead(i);
						}
						tmp = 0;
						for (int r = 0; r < Width; r++) {
							if (mapdata->checknxtMap({ r, ch.second }) == 'E') {
								tmp++;
							}
						}
						for (int c = 0; c < Height; c++) {
							if (mapdata->checknxtMap({ ch.first, c }) == 'E') {
								tmp++;
							}
						}
						if (tmp > 0) character->toDead();
					}
					else if (Effect[32] == 1) {
						double X = 0, Y = 0;
						for (int i = 0; i < enemylist->size(); i++) {
							X += enemylist->givePos(i).first;
							Y += enemylist->givePos(i).second;
						}
						X += ch.first; Y += ch.second; X *= 0.4; Y *= 0.4;
						int dX = floor(X), cX = ceil(X), dY = floor(Y), cY = ceil(Y);
						std::pair<int, int> pos;
						for (int i = 0; i < enemylist->size(); i++) {
							pos = enemylist->givePos(i);
							if (pos.first == dX || pos.first == cX || pos.second == dY || pos.second == cY)
								continue;
							enemylist->toDead(i);
						}
						if (ch.first != dX && ch.first != cX && ch.second != dY && ch.second != cY)
							character->toDead();
					}
					else if (Effect[32] == 2) {
						int tmp; std::pair<int, int> pos;
						for (int i = 0; i < enemylist->size(); i++) {
							tmp = 0; pos = enemylist->givePos(i);
							for (int r = pos.first - 3; r <= pos.first + 3; r++) {
								for (int c = pos.second - 3; c <= pos.second + 3; c++) {
									if (r < 0 || c < 0 || r >= Width || c >= Height)
										continue;
									if (mapdata->checknxtMap({ r,c }) == 'E' || mapdata->checknxtMap({ r,c }) == 'C') {
										tmp++;
									}
								}
							}
							if (tmp < 3) enemylist->toDead(i);
						}
						tmp = 0;
						for (int r = ch.first - 3; r <= ch.first + 3; r++) {
							for (int c = ch.second - 3; c <= ch.second + 3; c++) {
								if (r < 0 || c < 0 || r >= Width || c >= Height)
									continue;
								if (mapdata->checknxtMap({ r,c }) == 'E') {
									tmp++;
								}
							}
						}
						if (tmp < 2) character->toDead();
					}
					Effect[32] = std::rand() % 3;
				}
			}
			break;
			}
		case 33:
			item->deleteItem();
			break;
		case 34:
			item->deleteItem();
			break;
		case 40:
			item->deleteItem();
			score->deleteScore();
			knife = ch;
			switch (gamedata->lastkey) {
				case Up: knife.second--; break;
				case Down: knife.second++; break;
				case Left: knife.first--; break;
				case Right: knife.first++; break;
			}
			if (knife.first < 0 || knife.first >= Width || knife.second < 0 || knife.second >= Height) {
				break;
			}
			for (int i = 0; i < enemylist->size(); i++) {
				if (enemylist->givePos(i) == knife) {
					enemylist->toDead(i);
				}
			}
			break;
		case 41:
			for (int i = 0; i < enemylist->size(); i++) {
				if (score->isGetScore(enemylist->givePos(i))) {
					gamedata->Score_count--;
					view->SetEffect(enemylist->givePos(i), 'E', 12);
				}
				if (item->isGetItem(enemylist->givePos(i))) {
					Count[41]++;
					view->SetEffect(enemylist->givePos(i), 'E', 12);
				}
			}
			while (gamedata->Item_count > 0 && Count[41] > 0) {
				Count[41]--; gamedata->Item_count--;
			}
			while(Count[41] >= 2) {
				Count[41] -= 2;
				for (int i = 0; i < enemylist->size(); i++) {
					enemylist->Move(i, ch);
				}
			}
			for (int i = 0; i < 5; i++) {
				for (std::pair<int, int> tmp : score->givePos()) {
					enemylist->inputtarget(tmp);
				}
				for (std::pair<int, int> tmp : item->givePos()) {
					enemylist->inputtarget(tmp);
				}
			}
			break;
		case 42:
		{	
			std::pair<int, int> pos, lastpos, tmp;
			if (Count[42] == 0) Count[42] = 10;
			else if (Count[42] > 0) {
				Count[42]--;
				if (Count[42] == 0) {
					Count[42] = 10;
					for (int i = 0; i < enemylist->size(); i++) {
						pos = enemylist->givePos(i);
						lastpos = enemylist->givelastPos(i);
						if (pos == lastpos) continue;
						if (pos.first == lastpos.first) {
							if (pos.second < lastpos.second) {
								tmp = { pos.first, pos.second - 1 };
							}
							else{
								tmp = { pos.first, pos.second + 1 };
							}
						}
						else {
							if (pos.first < lastpos.first) {
								tmp = { pos.first - 1, pos.second };
							}
							else {
								tmp = { pos.first + 1, pos.second };
							}
						}
						if (tmp == ch) {
							character->toDead();
						}
						else if (tmp.first >= 0 && tmp.first < Width && tmp.second >= 0 && tmp.second < Height) {
							enemyknife.push_back(tmp);
						}
					}
				}
			}
			if (Effect[42] == 1) {
				Effect[42] = 0;
				knife = ch;
				switch (gamedata->lastkey) {
				case Up: knife.second--; break;
				case Down: knife.second++; break;
				case Left: knife.first--; break;
				case Right: knife.first++; break;
				}
				if (knife.first < 0 || knife.first >= Width || knife.second < 0 || knife.second >= Height) {
					break;
				}
				for (int i = 0; i < enemylist->size(); i++) {
					if (enemylist->givePos(i) == knife) {
						enemylist->toDead(i);
					}
				}
				if (knife.first < 0 || knife.first >= Width || knife.second < 0 || knife.second >= Height) {
					break;
				}
				if (mapdata->checknxtMap(knife) == 'E' || mapdata->checknxtMap(knife) == 'C')
					break;
				view->SetEffect(knife, '/', 4);
			}

			break;
		}
		case 43:
			Count[43]++;
			if (Count[43] < 1000) {
				if (mapdata->checknxtMap(ch) == 'C') {
					view->SetEffect(ch, '.', 7);
				}
				character->setPos({ Width / 2, Height / 2 });
				character->setUnDead();
			}
			else {
				item->deleteItem();
				score->deleteScore();
				gamedata->Score_count = gamedata->Item_count = 0;
			}
			break;
		case 44:
			if (gamedata->isGetItem) {
				gamedata->Item_count = max(0, gamedata->Item_count - 2);
			}
			if (Count[44] == 0) Count[44] = 20;
			else if (Count[44] > 0) {
				Count[44]--;
				if (Count[44] == 0) {
					Count[44] = 20;
					gamedata->Item_count++;
					if (gamedata->Item_count == 5) {
						gamedata->Item_count = 0;
						character->toDead();
					}
				}
			}
			break;
		case 45:
			item->deleteItem();
			if (Count[45] == 0) {
				Count[45] = 50;
				Effect[45] = 0;
				gamedata->Item_count = 0;
			}
			else if (Count[45] > 0) {
				Count[45]--;
				gamedata->Item_count = (4-Count[45] / 10);
				if (Count[45] == 0) {
					Count[45] = 50;
					Effect[45]++;
					if (gamedata->Score_count < Effect[45] * 3) {
						character->toDead();
					}
				}
			}
			break;
		case 46:
			if (gamedata->isGetItem) {
				gamedata->Item_count = max(0, gamedata->Item_count - 2);
			}
			if(Count[46] == 0) Count[46] = 20;
			else if (Count[46] > 0) {
				Count[46]--;
				if (Count[46] == 0) {
					Count[46] = 20;
					gamedata->Item_count = min(5, gamedata->Item_count + 1);
				}
			}
			Sleep(200 * gamedata->Item_count);
			break;
		case 47:
		{
			if (Count[47] == 0) {
				enemylist->clear();
				for (int c = 0; c < Height; c += 4) {
					enemylist->push_back({ 0, c }, std::rand() % 5, mapdata);
					enemylist->push_back({ Width - 1, c }, std::rand() % 5, mapdata);
				}
			}
			else if (Count[47] == 100) {
				enemylist->clear();
				for (int r = 2; r < Width; r += 5) {
					for (int c = 2; c < Height; c += 5) {
						enemylist->push_back({ r,c }, 4, mapdata);
					}
				}
			}
			else if (Count[47] == 200) {
				enemylist->clear();
			}
			else if (Count[47] > 200 && Count[47] < 300) {
				if (Count[47] % 5 == 0)
					enemylist->push_back(mapdata->giveLegalPos({ 0,0,Width,Height }, ch), std::rand() % 5, mapdata);
			}
			else if (Count[47] == 300) {
				enemylist->clear();
			}
			else if (Count[47] > 300) {
				int dx[4] = { 0,0,1,-1 }, dy[4] = { 1,-1,0,0 };
				for (int i = 0; i < 4; i++) {
					if (ch.first + dx[i] < 0 || ch.first + dx[i] >= Width)
						continue;
					if (ch.second + dy[i] < 0 || ch.second + dy[i] >= Height)
						continue;
					enemylist->push_back({ ch.first + dx[i], ch.second + dy[i] }, 1, mapdata);
				}
				gamedata->Life_count = 1;
				gamedata->Item_count = 0;
				character->toDead();
			}
			if (gamedata->Item_count >= 3 && mapdata->isInDanger(ch)) {
				gamedata->Item_count -= 3;
				character->setUnDead();
			}
			Count[47]++;
			break;
		}
		case 50:
			if (gamedata->Round_count == 1) {
				now = clock();
				if (now - standard >= 1000*600) {
					Effect[50] = 1;
					SpecialItem = mapdata->giveLegalPos({ 0,0,Width,Height });
				}
			}
			if (Effect[50] == 1) {
				item->deleteItem();
				score->deleteScore();
				character->setUnDead();
				for (int i = 0; i < enemylist->size(); i++) {
					enemylist->setParalyze(i, 5);
				}
				message->input("Game Clear");

				if (ch == SpecialItem) {
					SpecialItem = { -1, -1 };
					gamedata->Score_count = 999;
				}
			}
			break;
	}
	DrawEffect();
}


void Ability::DrawEffect() {
	std::pair<int, int> ch = character->givePos();
	switch (*AbilityNo) {
		case 0:
			for (int r = 0; r < Width; r++) {
				for (int c = 0; c < Height; c++) {
					if (grid_0[r][c]>0) {
						view->SetEffect({ r,c }, '0' + grid_0[r][c], 13);
					}
				}
			}
			break;
		case 1:
			if (Effect[1]) view->SetQuickEffect(character->givePos(), 'C', 3);
			break;
		case 3:
			if (Effect[3] == 1) {
				for (int i = 0; i < enemylist->size(); i++) {
					if (character->givePos() == enemylist->givePos(i)) {
						view->SetQuickEffect(character->givePos(), 'C', 75);
					}
					else {
						view->SetQuickEffect(enemylist->givePos(i), 'E', 13);
					}
				}
			}
			break;
		case 4:
			if (Count[4]>0) {
				Count[4]--;
				for (int i = 0; i < enemylist->size(); i++) {
					view->SetEffect(enemylist->givePos(i), 'E', 5);
				}
			}
			break;
		case 10:
			if (Effect[10] > 0) {
				if (Count[10] > 0) view->SetEffect(character->givePos(), 'C', 43);
				for (int i = 0; i < enemylist->size(); i++) {
					if (enemylist->checkCountDown(i)) {
						view->SetEffect(enemylist->givePos(i), 'E', 5);
					}
				}
			}
			break;
		case 11:
			if (Effect[11] == 1) {
				for (int i = 0; i < enemylist->size(); i++) {
					if (character->givePos() == enemylist->givePos(i)) {
						view->SetQuickEffect(character->givePos(), 'C', 75);
					}
					else {
						view->SetQuickEffect(enemylist->givePos(i), 'E', 13);
					}
					view->SetQuickEffect(enemylist->givelastPos(i), '.', 7);
				}
			}
			break;
		case 13:
			if (Effect[13] == 1) {
				view->SetEffect(character->givePos(), 'C', 59);
			}
			break;
		case 14:
			if (Count[14] != 0) {
				for(int i=0; i<enemylist->size(); i++){
					view->SetEffect(enemylist->givePos(i), '.', 7);
				}
			}
			break;
		case 15:
			if (Effect[15] <= 10) {
				view->SetEffect(character->givePos(), 'C', 43);
			}
			break;
		case 21:
			for (int i = 0; i < enemylist->size(); i++) {
				if (enemylist->checkisParalyze(i)) {
					mapdata->Update_DangerMap(enemylist->givePos(i));
					if (enemylist->checkisParalyze(i) == 1) {
						if (enemylist->givePos(i) == character->givePos()) {
							view->SetQuickEffect(character->givePos(), 'C', 43);
						}
						else {
							view->SetQuickEffect(enemylist->givePos(i), ' ', 34);
						}
						view->SetClear(enemylist->givePos(i));
					}
					else if (enemylist->checkisParalyze(i) == 30) {
						view->SetEffect(enemylist->givePos(i), 'E', 252);
					}
					else {
						view->SetClear(enemylist->givePos(i));
					}
				}
			}
			for (std::pair<int, int> tmp : Q_21) {
				view->SetQuickEffect(tmp, 'E', 8);
				view->SetClear(tmp);
			}
			break;
		case 23:
			if (Effect[23] == 1) {
				for (int i = 0; i < enemylist->size(); i++) {
					view->SetEffect(enemylist->givePos(i), 'E', 7);
					
				}
				view->SetEffect(character->givePos(), 'C', 7);
				for (std::pair<int, int> tmp : item->givePos()) {
					view->SetEffect(tmp, 'I', 8);
				}
			}
			break;
		case 32:
			if (Count[32] <= 10) {
				view->SetEffect(character->givePos(), 'C', 43);
			}
			break;
		case 34:
		{
			for (std::pair<int, int> tmp : score->givePos()) {
				view->SetEffect(tmp, 'S', 14);
			}
			for (int r = 0; r < Width; r++) {
				for (int h = 0; h < Height; h++) {
					if (mapdata->checkObstacle({ r,h }))
						continue;
					view->SetClear({ r,h });
					if ((ch.first - 3 <= r && r <= ch.first + 3) && (ch.second - 3 <= h && h <= ch.second + 3)) {
						continue;
					}
					view->SetEffect({ r,h }, '.', 136);
				}
			}
			break;
		}
		case 40:
		{
			if (knife.first < 0 || knife.first >= Width || knife.second < 0 || knife.second >= Height) {
				break;
			}
			if (mapdata->checknxtMap(knife) == 'E' || mapdata->checknxtMap(knife) == 'C')
				break;
			view->SetEffect(knife, '/', 4);
			break;
		}
		case 42:
			for (std::pair<int, int> tmp : enemyknife) {
				view->SetEffect(tmp, '/', 4);
			}
			break;
		case 43:
			view->SetEffect(ch, 'C', 13);
			break;
		case 47:
		{
			if (Count[47] > 301) {
				int dx[4] = { 0,0,1,-1 }, dy[4] = { 1,-1,0,0 };
				std::pair<int, int> tmp;
				view->SetQuickEffect(ch, 'C', 11);
				for (int i = 0; i < 4; i++) {
					tmp = { ch.first + dx[i], ch.second + dy[i]};
					if (tmp.first < 0 || tmp.first >= Width || tmp.second < 0 || tmp.second >= Height)
						continue;
					view->SetQuickEffect(tmp, '.', 7);
				}
				for (int delta = 21; delta >= 1; delta--) {
					for (int i = 0; i < 4; i++) {
						tmp = { ch.first + dx[i] * delta, ch.second + dy[i] * delta };
						if (tmp.first < 0 || tmp.first >= Width || tmp.second < 0 || tmp.second >= Height)
							continue;
						view->SetQuickEffect(tmp, 'E', 12);
					}
					Sleep(150);
					for (int i = 0; i < 4; i++) {
						tmp = { ch.first + dx[i] * delta, ch.second + dy[i] * delta };
						if (tmp.first < 0 || tmp.first >= Width || tmp.second < 0 || tmp.second >= Height)
							continue;
						view->SetQuickEffect(tmp, '.', 7);
					}
				}
			}
			break;
		}
		case 50:
		{
			if (Effect[50] == 1 && gamedata->Round_count == 1) {
				for (int i = 1; i <= 12; i++) {
					for (int r = ch.first - i; r <= ch.first + i; r++) {
						for (int c = ch.second - i; c <= ch.second + i; c++) {
							if (r < 0 || r >= Width || c < 0 || c >= Height)
								continue;
							if (r != ch.first - i && r != ch.first + i && c != ch.second - i && c != ch.second + i)
								continue;
							view->SetQuickEffect({ r,c }, '.', 119);
						}
					}
					view->SetQuickEffect(ch, 'C', 11);
					Sleep(100);
					for (int r = ch.first - i; r <= ch.first + i; r++) {
						for (int c = ch.second - i; c <= ch.second + i; c++) {
							if (r < 0 || r >= Width || c < 0 || c >= Height)
								continue;
							if (r != ch.first - i && r != ch.first + i && c != ch.second - i && c != ch.second + i)
								continue;
							view->SetQuickEffect({ r,c }, '.', 7);
						}
					}
					std::pair<int, int> pos;
					for (int s = 0; s < enemylist->size(); s++) {
						pos = enemylist->givePos(s);
						if (ch.first - i <= pos.first && pos.first <= ch.first + i &&
							ch.second - i <= pos.second && pos.second <= ch.second + i) {
							view->SetQuickEffect(pos, 'E', 8);
						}
					}
				}
			}
			if (Effect[50] == 1) {
				for (int i = 0; i < enemylist->size(); i++) {
					view->SetEffect(enemylist->givePos(i), 'E', 8);
				}
				view->SetEffect(ch, 'C', 11);
				if (SpecialItem.first != -1 && SpecialItem.second != -1) {
					view->SetEffect(SpecialItem, 'S', 13);
				}
			}
			break;
		}
	}
}