#pragma once
const int ACTION_UP = 0;
const int ACTION_DOWN = 1;
const int ACTION_LEFT = 2;
const int ACTION_RIGHT = 3;
const int REWARD = -1;

const int MAP[8][8] = {
				{1, 1, 1, 1, 1, 1, 1, 1},
				{1, 0, 0, 0, 0, 0, 0, 1},
				{1, 0, 0, 0, 0, 0, 0, 1},
				{1, 1, 1, 1, 1, 0, 0, 1},
				{1, 0, 0, 0, 0, 0, 0, 1},
				{1, 0, 0, 0, 0, 0, 0, 1},
				{1, 0, 0, 0, 0, 0, 0, 1},
				{1, 1, 1, 1, 1, 1, 1, 1 }
};

const int X = 8;
const int Y = 8;
//const int SIZE[2] = { X, Y };

const int ACTIONS[4] = { ACTION_UP, ACTION_DOWN, ACTION_LEFT, ACTION_RIGHT };
const int ACTION_SPACE = 4;
const int START[2] = { 1, 1 };
const int GOAL[2] = { X - 2, Y - 4 };

class gridWorld {

public:           // Access specifier
	int size = X;
	int state[2] = { -1, -1 };
	bool done = false;
	int reward = 999;
	int numActions = ACTION_SPACE;
	// The class
	gridWorld() {     // Constructor
		reset();
	}

	void step(int action);

	void pos(int action);

	void reset(void);

	void toString(void);


};