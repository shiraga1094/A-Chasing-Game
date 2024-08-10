#include <iostream>
#include "GameControl.h"

GameControl game;

int main() {
	srand(time(NULL));
	game.Run();
}