#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "GameControl.h"

GameControl game;


int main() {
	srand(time(NULL));
	game.Run();
	
}