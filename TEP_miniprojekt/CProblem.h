#pragma once
#include "Clause.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <math.h>

class CIndividual;

class CProblem
{
private:
	std::vector<Clause*> all_clauses;
public:
	CProblem() {};
	CProblem(const CProblem& other);
	~CProblem();

	bool bLoad(std::string file_name);
	int compute(CIndividual* some_solution);

	std::vector< Clause* >* getClauses();

	void printClauses();

};

