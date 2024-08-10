#ifndef _Message_
#define _Message_

#include <iostream>
#include <vector>
#include "CursorControl.h"
#include "MapData.h"

const int BaseX = Width * 2 + 1;
const int BaseY = 4;
class Message {
private:
	int* AbilityNo;
	int* GameModeNo;

	std::string AbilityName;
	std::vector<std::string> AbilityIntro;
	std::vector<std::string> Command;
public:
	Message(int* _AbilityNo, int* _GameModeNo);
	void outputName();
	void inputName(std::string name, std::vector<std::string> intro);
	void input(std::string tmp);
	void output();
};

#endif

