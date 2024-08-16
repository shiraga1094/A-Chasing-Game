#include "Achievement.h"

Achievement::Achievement(int* _AbilityNo, int* _GameModeNo) {
	GameModeNo = _GameModeNo;
	AbilityNo = _AbilityNo;
	for (int i = 0; i < 100; i++) {
		data[i].isAchieve = true;
		if (i >= 40) data[i].isAchieve = false;
		for (int s = 0; s < 4; s++) {
			data[i].Max_Kill[s] = 0;
			data[i].Max_Round[s] = 0;
			data[i].Max_Round[s] = 0;
		}
	}
	std::ifstream keydata("Data/key.kfile", std::ios::in | std::ios::binary);
	if (!keydata) {
		std::cout << "key.kfile not found.";
		std::abort();
	}
	key = 0;
	while (!keydata.eof()) {
		key += keydata.get();
	}
	keydata.close();
}
bool Achievement::Decryption() {
	std::ifstream input("Data/PlayerData.dat", std::ios::in | std::ios::binary);
	if (!input) {
		return false;
	}
	
	std::ofstream output("Data/GameData.dat", std::ios::out | std::ios::binary);

	while (input.good() && output.good()) {
		output.put(input.get() ^ key);
	}
	input.close();
	output.close();
	return true;
}
void Achievement::Encryption() {
	std::ifstream input("Data/GameData.dat", std::ios::in | std::ios::binary);
	std::ofstream output("Data/PlayerData.dat", std::ios::out | std::ios::binary);

	if (!input) {
		std::cout << "GameData.dat not found.";
		std::abort();
	}

	while (input.good() && output.good()) {
		output.put(input.get() ^ key);
	}
	input.close();
	output.close();
	remove("Data/GameData.dat");
}
void Achievement::Input() {
	if (!Decryption()) return;
	std::ifstream input("Data/GameData.dat", std::ios::in|std::ios::binary);
	if (!input) {
		throw "GameData.dat not found.";
	}
	int tmp;
	for (int i = 0; i < 100; i++) {
		for (int s = 0; s < 4; s++) {
			input >> tmp; data[i].Max_Score[s] = tmp ^ key;
			input >> tmp; data[i].Max_Item[s] = tmp ^ key;
			input >> tmp; data[i].Max_Kill[s] = tmp ^ key;
			input >> tmp; data[i].Max_Round[s] = tmp ^ key;
		}
		input >> tmp; data[i].isAchieve = tmp ^ key;
	}
	input.close();
	remove("Data/GameData.dat");
}
void Achievement::Output() {
	std::ofstream output("Data/GameData.dat", std::ios::out|std::ios::binary);
	
	for (int i = 0; i < 100; i++) {
		for (int s = 0; s < 4; s++) {
			output << (data[i].Max_Score[s] ^ key);
			output << ' ';
			output << (data[i].Max_Item[s] ^ key);
			output << ' ';
			output << (data[i].Max_Kill[s] ^ key);
			output << ' ';
			output << (data[i].Max_Round[s] ^ key);
			output << ' ';
		}
		output << (data[i].isAchieve ^ key);
		output << ' ';
	}

	output.close();
	Encryption();
}
void Achievement::Update(GameData* gamedata) {
	data[*AbilityNo].Max_Score[*GameModeNo] = std::max(gamedata->Score_count,data[*AbilityNo].Max_Score[*GameModeNo]);

	data[*AbilityNo].Max_Item[*GameModeNo] = std::max(gamedata->Max_Item_count,data[*AbilityNo].Max_Item[*GameModeNo]);

	data[*AbilityNo].Max_Kill[*GameModeNo] = std::max(gamedata->Kill_count,data[*AbilityNo].Max_Kill[*GameModeNo]);

	data[*AbilityNo].Max_Round[*GameModeNo] = std::max(gamedata->Round_count,data[*AbilityNo].Max_Round[*GameModeNo]);

	UnlockCheck(gamedata);
}
void Achievement::UnlockCheck(GameData* gamedata) {
	if (gamedata->Kill_count > 99) {
		data[40].isAchieve = true;
	}
	if (gamedata->Round_count > 1000) {
		data[43].isAchieve = true;
	}
	if (gamedata->Max_Item_count > 99) {
		data[44].isAchieve = true;
	}
	if (gamedata->Max_Score_count > 99) {
		data[45].isAchieve = true;
	}

	bool tmp = false;
	int testify[10] = { 20, 21, 22, 23, 24, 30, 31, 32, 33, 34 };
	for (int i = 0; i < 10; i++) {
		for (int s = 0; s < 4; s++) {
			if (data[testify[i]].Max_Score[s] >= 10)
				tmp = true;
			else
				tmp = false;
		}
		if (tmp == false) {
			break;
		}
	}
	if (tmp == true) {
		data[47].isAchieve = true;
	}
	for (int i = 0; i < 3; i++) {
		if (data[47].Max_Round[i] > 300 && data[47].Max_Score[i] >= 10)
			data[50].isAchieve = true;
	}
	tmp = false;
	int test[10] = {40, 41, 42, 43, 44, 45, 46, 47, 50, -1};
	for (int i = 0; i < 9; i++) {
		if (data[test[i]].isAchieve) {
			tmp = true;
		}
		else {
			tmp = false;
		}
		if (tmp == false)
			break;
	}
	if (tmp == true)
		data[51].isAchieve = true;
}
bool Achievement::CheckisAchieve(int No) {
	return data[No].isAchieve;
}
bool Achievement::CheckLevelAchieve(int level) {
	for (int i = level * 10; i < (level + 1) * 10; i++) {
		if (data[i].isAchieve)
			return true;
	}
	return false;
}
void Achievement::CheckSpecialUnlock(GameData* gamedata) {
	if (*AbilityNo == 30) {
		if (gamedata->Score_count == 10 && gamedata->Enemy_getItem_count <= 2) {
			data[41].isAchieve = true;
		}
	}
	if (gamedata->Kill_count-gamedata->last_Kill_count>=20) {
		data[42].isAchieve = true;
	}
	if (gamedata->Round_count == 300 && gamedata->isPerfect) {
		data[46].isAchieve = true;
	}
}
Achieve_Data Achievement::giveAchieveData() {
	return data[*AbilityNo];
}