#include<string>
#include <vector>
#include<cmath>
#include <iostream>
#include "gridWorld.h"
using namespace std;




void gridWorld::step(int action) {

	pos(action);
	if (state[0] == GOAL[0] && state[1] == GOAL[1]) {
		done = true;
		reward = 0;
	}
	else {
		reward = REWARD;
	}
}
void gridWorld::pos(int action) {

	int i = state[0];
	int j = state[1];

	if (action == ACTION_UP) {
		state[0] = i - 1 + MAP[i - 1][j];
		state[1] = j;
	}
	else if (action == ACTION_DOWN)
	{
		state[0] = i + 1 - MAP[i + 1][j];
		state[1] = j;
	}
	else if (action == ACTION_LEFT)
	{
		state[0] = i;
		state[1] = j - 1 + MAP[i][j - 1];
	}
	else if (action == ACTION_RIGHT)
	{
		state[0] = i;
		state[1] = j + 1 - MAP[i][j + 1];
	}
}


void gridWorld::reset(void) {
	state[0] = START[0];
	state[1] = START[1];
	done = false;
}

void gridWorld::toString(void) {
	for (int i = 0; i < X; i++) {
		std::cout << " _";
	}
	for (int i = 0; i < X; i++) {
		std::cout << endl;
		for (int j = 0; j < X; j++) {
			if (state[0] == i && state[1] == j) {
				std::cout << "|O";
			}
			else if (GOAL[0] == i && GOAL[1] == j) {
				std::cout << "|X";
			}
			else {
				if (MAP[i][j] == 1)
					std::cout << "|Z";
				else
					std::cout << "|_";

			}



		}
		std::cout << "|";
	}
	std::cout << endl;

	
}
