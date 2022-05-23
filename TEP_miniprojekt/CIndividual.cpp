#include "CIndividual.h"

using namespace std;

CProblem* CIndividual::problem;
map< int, int > CIndividual::best_solution;

CIndividual::CIndividual() {
	auto my_seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	rand_engine.seed(my_seed);

	problem = new CProblem();
}

CIndividual::CIndividual(CProblem* something) {
	auto my_seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	rand_engine.seed(my_seed);

	problem = new CProblem(*something);
}

CIndividual::CIndividual(const CIndividual& other) {

	auto my_seed = chrono::high_resolution_clock::now().time_since_epoch().count();
	rand_engine.seed(my_seed);

	problem = new CProblem(*other.problem);

	pair<int, int> tmp;
	for (auto it = other.solution.begin(); it != other.solution.end(); it++) {
		tmp.first = it->first;
		tmp.second = it->second;
		solution.insert(tmp);
	}
}

CIndividual::~CIndividual() {}

map< int, int >* CIndividual::getSolution() { return &solution; }

// iterating through the whole solution and randomly choosing the source of every gene one at a time
CIndividual* CIndividual::crossover(CIndividual* coparent) {

	uniform_int_distribution< int > int_dist(0, 1);
	int rand = -1;

	map<int, int>::iterator it_this;
	map<int, int>::iterator it_coparent;

	pair<int, int> tmp;

	CIndividual* child = new CIndividual(problem);

	for (it_this = solution.begin(), it_coparent = coparent->solution.begin();
			it_this != solution.end() && it_coparent != coparent->solution.end();
				it_this++, it_coparent++) {

		rand = int_dist(rand_engine);
		if (rand == 0) {
			tmp.first = it_this->first;
			tmp.second = it_this->second;
			child->solution.insert(tmp);
		}
		else {
			tmp.first = it_coparent->first;
			tmp.second = it_coparent->second;
			child->solution.insert(tmp);
		}

	}
	return child;
}

// iterating through the whole solution and assessing mutation on each gene
void CIndividual::mutation(double mutation_chance) {

	map<int, int>::iterator it;
	uniform_real_distribution<float> float_dist(0, 1);
	float dice;

	for (it = solution.begin(); it != solution.end(); it++) {

		dice = float_dist(rand_engine);
		if (dice <= mutation_chance) {

			if (it->second == satisfied) {
				it->second = dissatisfied;
			}
			else {
				it->second = satisfied;
			}

		}

	}

}

int CIndividual::getFitness() { return problem->compute(this); }

CProblem* CIndividual::getProblem() { return problem; }

int* CIndividual::getResultAsTable() {

	map< int, int >::iterator it;
	int index;
	int size = best_solution.size();
	int* result = new int[size];

	for (it = best_solution.begin(), index = 0;
		it != best_solution.end() && index < size;
		it++, index++) {

		result[index] = it->second;

	}
	return result;

}

map< int, int >* CIndividual::getBestSolution() { return &best_solution; }
