#include "Message.h"

Message::Message(int* _AbilityNo, int* _GameModeNo){
	AbilityNo = _AbilityNo;
	GameModeNo = _GameModeNo;
	Command.push_back("Command Output:");
}
void Message::inputName(std::string name, std::vector<std::string> intro) {
	AbilityName = name;
	AbilityIntro = intro;
}
void Message::input(std::string tmp) {
	Command.push_back(tmp);
}
void Message::outputName() {
	gotoxy(BaseX, BaseY); std::cout << AbilityName;
	for (int i = 0; i<AbilityIntro.size(); i++) {
		gotoxy(BaseX, BaseY + 2 + i); std::cout << AbilityIntro[i];
	}
}
void Message::output() {
	for (int c = BaseY + 7; c <= BaseY+12; c++) {
		gotoxy(BaseX, c); std::cout << "                                       ";
	}
	int len = Command.size(), delta = 0;
	for (int i = 0; i < len; i++) {
		gotoxy(BaseX, BaseY + 6 + i + delta);
		for (int s = 0; s < Command[i].length(); s++) {
			if (Command[i][s] == '#') {
				delta++;
				gotoxy(BaseX, BaseY + 6 + i + delta);
			}
			else {
				std::cout << Command[i][s];
			}
		}
	}
	Command.clear();
	Command.push_back("Command Output:");
}