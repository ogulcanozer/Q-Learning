#include "gridWorld.h"
#include "qLearning.h"
#include <iostream>

int main() {

	gridWorld g; 
	qLearning q;
	int numEpisodes = 0;
	std::cout << "Number of episodes to train : ";
	std::cin >> numEpisodes;	
	while (std::cin.fail()) {
		std::cout << "Error."<<std::endl;
		std::cin.clear();
		std::cin.ignore(256, '\n');
		std::cin >> numEpisodes;
	}
	//Start.
	std::map<int, std::vector<double>> table = q.q_learning( &g , numEpisodes ,  1.0,  0.5,  0.1);


	return 0;
}