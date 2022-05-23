#include "COptimizer.h"

using namespace std;

vector< CIndividual* > COptimizer::next_generation;
int COptimizer::highest_fitness_overall;

COptimizer::COptimizer() {
	population_size = DEFAULT_POP_SIZE;
	crossover_chance = DEFAULT_CROSS_CHANCE;
	mutation_chance = DEFAULT_MUT_CHANCE;

	highest_fitness_overall = 0;

	auto my_seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	rand_engine.seed(my_seed);
}

COptimizer::COptimizer(int pop, double cross, double mut) {
	
	if (pop <= 0)  population_size = DEFAULT_POP_SIZE; 
	else population_size = pop;

	if (cross < 0 || cross > 1) crossover_chance = DEFAULT_CROSS_CHANCE;
	else crossover_chance = cross;

	if (mut < 0 || mut > 1) mutation_chance = DEFAULT_MUT_CHANCE;
	else mutation_chance = mut;

	highest_fitness_overall = 0;

	auto my_seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	rand_engine.seed(my_seed);
}

COptimizer::~COptimizer() {

	for (unsigned int i = 0; i < next_generation.size(); i++) {
		auto duplicate = find(population.begin(), population.end(), next_generation[i]);
		// it means that it is not the same, and i can safely delete it
		if (duplicate == population.end()) {
			delete next_generation[i];
		}
	}

	for (unsigned int i = 0; i < population.size(); i++) {
		delete population[i];
	}

}

bool COptimizer::initialize(string file_name) {

	CProblem* max3sat = new CProblem();
	bool check = max3sat->bLoad(file_name);
	if (check == false) {
		cout << "\nSomething went wrong while loading clauses from the file. You must have entered a wrong file name" << endl;
		return false;
	}

	max3sat->printClauses();

	uniform_int_distribution< int > int_dist(0, 1);
	pair< int, int > input;

	for (unsigned int i = population.size(); i < population_size; i++) {

		population.push_back(new CIndividual(max3sat));		
		
		for (unsigned int j = 0; j < population[i]->problem->getClauses()->size(); j++) {

			for (unsigned int k = 0; k < 3; k++) {
				input.first = abs( population[i]->problem->getClauses()->at(j)->getSignatures()->at(k) );
				input.second = int_dist(rand_engine);
				population[i]->getSolution()->insert(input);
			}

		}

	}
	return true;
}

CIndividual* COptimizer::evaluateParents(CIndividual* parent1, CIndividual* parent2) {
	if (parent1->getFitness() > parent2->getFitness())
		return parent1;
	else
		return parent2;
}

CIndividual* COptimizer::tournamentSelection() {

	uniform_int_distribution< int > int_dist(0, population_size - 1);
	int dice_1 = int_dist(rand_engine);
	int dice_2 = int_dist(rand_engine);

	if (dice_1 == dice_2)
		return population[dice_1];
	else
		return evaluateParents(population[dice_1], population[dice_2]);

}

void COptimizer::runIteration() {

	uniform_real_distribution< float > float_dist(0, 1);
	uniform_int_distribution< int > int_dist(0, 1);
	int zero_one;
	float dice;

	next_generation.clear();

	while (next_generation.size() < population_size) {

		CIndividual* parent1 = NULL;
		CIndividual* parent2 = NULL;
		CIndividual* child = NULL;

		do {
			parent1 = tournamentSelection();
			parent2 = tournamentSelection();
		} while (parent1 == parent2);
		
		dice = float_dist(rand_engine);

		if (dice <= crossover_chance) {
			child = parent1->crossover(parent2);
			child->mutation(mutation_chance);
			next_generation.push_back(child);
		}
		else {
			// it would mean that there is space for only one of the parents
			if (next_generation.size() == population_size - 1) {
				zero_one = int_dist(rand_engine);

				if (zero_one == 0) {
					//parent1->mutation(mutation_chance);
					next_generation.push_back(parent1);
				}
				else {
					//parent2->mutation(mutation_chance);
					next_generation.push_back(parent2);
				}

			}
			// standard case
			else {
				//parent1->mutation(mutation_chance);
				//parent2->mutation(mutation_chance);
				next_generation.push_back(parent1);
				next_generation.push_back(parent2);
			}
		}

	}

	/*
	for (unsigned int i = 0; i < population.size(); i++) {
		auto duplicate = find(next_generation.begin(), next_generation.end(), population[i]);
		// it means that it is not the same, and i can safely delete it
		if (duplicate == next_generation.end()) {
			delete population[i];
		}
	}
	*/
	population = next_generation;

	evaluateProgress();
}

void COptimizer::evaluateProgress() {

	int best = -1;
	int current = -1;
	int index = -1;
	for (unsigned int i = 0; i < population.size(); i++) {
		current = population[i]->getFitness();
		if (current > best) {
			best = current;
			index = i;
		}
	}

	if (best > highest_fitness_overall) {
		CIndividual::best_solution = population[index]->solution;
		highest_fitness_overall = best;
	}

	cout << " - Currently highest fitness: " << highest_fitness_overall << " / " <<
		population[index]->getProblem()->getClauses()->size() << endl;

}

int COptimizer::getHighestScore() { return highest_fitness_overall; }

CIndividual* COptimizer::getMember(int index) { return population[index]; }

vector< CIndividual* >* COptimizer::getNextGeneration() { return &next_generation; }