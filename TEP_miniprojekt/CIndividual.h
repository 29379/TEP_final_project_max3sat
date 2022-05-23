#pragma once

// these warnings were annoying and popped up in every constructor
// as i was setting up the seed for my random number generator engine,
// so i made them appear just once in every file as a remainder
#pragma warning ( once : 4244 )

#include "Clause.h"
#include "CProblem.h"
#include <map>
#include <random>
#include <chrono>
#include <time.h>

class CIndividual
{
protected:
	friend class COptimizer;

	enum effect {
		dissatisfied = 0, satisfied
	};
	// key - signature (without negation)
	// value - indicator if the variable is true or not
	std::map<int, int> solution;
	std::mt19937 rand_engine; 

	static CProblem* problem; 
	static std::map< int, int > best_solution;

public:
	
	CIndividual();
	CIndividual(CProblem* problem);
	CIndividual(const CIndividual& other);
	~CIndividual();

	CIndividual* crossover(CIndividual* coparent);
	void mutation(double mutation_chance);
	int getFitness();
	
	std::map< int, int >* getSolution();
	CProblem* getProblem();
	std::map< int, int >* getBestSolution();

	int* getResultAsTable();
};

