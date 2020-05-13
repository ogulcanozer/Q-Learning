#pragma once
#include <map>
#include <iostream>
#include "gridWorld.h"
#include <vector>
class qLearning {
	std::map< int, std::vector<double>> qTable;
public:
	int currentState = -1;
	int action = -1;
	qLearning() {
		/*std::map< int, std::vector<float>> qTable;*/

	}
	int epsilonGreedy(std::vector<double> observation, float epsilon, int numActions);
	int stateToTable(int* state, int size);
	std::map< int, std::vector<double>> q_learning(gridWorld* enviroment, int numEpisodes, float discount_factor = 1.0, float alpha = 0.5, float epsilon = 0.1);
	bool checkQTable(int state);
};