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
		gotoxy(BaseX, c); std::cout << "                                                 ";
	}
	for (int i = 0; i < Command.size(); i++) {
		gotoxy(BaseX, BaseY + 6 + i); std::cout << Command[i];
	}
	Command.clear();
	Command.push_back("Command Output:");
}