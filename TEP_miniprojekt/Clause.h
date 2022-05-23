#pragma once
#include <iostream>
#include <vector>
class CIndividual;
class Clause
{
	friend class CIndividual;
private:
	// signatures of every value (with negation)
	std::vector< int > signatures;
public:
	Clause() {};

	bool isSatisfied(CIndividual* current);

	std::vector< int >* getSignatures();
};

