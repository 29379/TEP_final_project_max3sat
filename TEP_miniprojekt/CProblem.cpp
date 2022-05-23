#include "CProblem.h"

using namespace std;

CProblem::CProblem(const CProblem& other) {
	copy(other.all_clauses.begin(), other.all_clauses.end(), back_inserter(all_clauses));
}

CProblem::~CProblem() {
	for (unsigned int i = 0; i < all_clauses.size(); i++) {
		delete all_clauses[i];
	}
}

vector< Clause* >* CProblem::getClauses() { return &all_clauses; }

bool CProblem::bLoad(string file_name) {

	string temp;
	int key;
	string line;
	Clause tmp;

	ifstream infile(file_name);
	if (!infile.is_open()) return false;

	while (getline(infile, line)) {

		istringstream iss(line);
		tmp.getSignatures()->clear();

		while (!iss.eof()) {

			iss >> temp;

			if (istringstream(temp) >> key) {
				tmp.getSignatures()->push_back(key);
			}

		}
		all_clauses.push_back(new Clause(tmp));
	}
	return true;

}

int CProblem::compute(CIndividual* some_solution) {
	
	int ret_value = 0;
	for (unsigned int i = 0; i < all_clauses.size(); i++) {
		if (all_clauses[i]->isSatisfied(some_solution)) {
			ret_value++;
		}
	}
	return ret_value;
}

void CProblem::printClauses() {

	for (unsigned int i = 0; i < all_clauses.size(); i++) {
		cout << i+1 << " - ( ";
		for (unsigned int j = 0; j < all_clauses[i]->getSignatures()->size(); j++) {
			cout << all_clauses[i]->getSignatures()->at(j) << " ";
		}
		cout << ")" << endl;
	}

}