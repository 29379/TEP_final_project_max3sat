#pragma once

#define DEFAULT_POP_SIZE 150
#define DEFAULT_CROSS_CHANCE 0.75
#define DEFAULT_MUT_CHANCE 0.075

#include "Clause.h"
#include "CProblem.h"
#include "CIndividual.h"
#include <random>
#include <time.h>
#include <iomanip>

class COptimizer
{
private:
	std::vector< CIndividual* > population;
	static std::vector< CIndividual* > next_generation;
	std::mt19937 rand_engine;

	static int highest_fitness_overall;

	unsigned int population_size;
	double crossover_chance;
	double mutation_chance;
public:
	COptimizer();
	COptimizer(int pop, double cross, double mut);
	~COptimizer();

	bool initialize(std::string file_name);
	CIndividual* tournamentSelection();
	CIndividual* evaluateParents(CIndividual* parent1, CIndividual* parent2);
	void runIteration();

	void evaluateProgress();
	
	int getHighestScore();
	CIndividual* getMember(int index);
	std::vector< CIndividual* >* getNextGeneration();

};

