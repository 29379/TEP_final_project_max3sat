#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define DEFAULT_ITER_COUNT 500

#include "CIndividual.h"
#include "Clause.h"
#include "COptimizer.h"
#include "CProblem.h"

using namespace std;

double getNumberFromTheUser() {

	char s[12];
	cin >> s;
	double result = 0;
	if (!sscanf(s, "%lf", &result) || result < 0) {
		cout << "\nThe number that you have entered is not correct; -1 was set by default" << endl;
		result = -1;
	}
	return result;

}

void execute(string file_name) {

	int count = 0;
	
	cout << endl << "Please type in the size of the population" << endl;
	int x = (int)getNumberFromTheUser();

	cout << endl << "Please type in the probability of crossover" << endl;
	double y = getNumberFromTheUser();

	cout << endl << "Please type in the probability of mutation" << endl;
	double z = getNumberFromTheUser();

	cout << endl << "Please type in the number of iterations you want to run" << endl;
	int iter_count = (int)getNumberFromTheUser();
	if (iter_count <= 0) iter_count = DEFAULT_ITER_COUNT;
	
	auto start = chrono::system_clock::now();

	COptimizer* opt = new COptimizer(x, y, z);
	bool check = opt->initialize(file_name);
	if (!check) { return; }

	int size = opt->getMember(0)->getProblem()->getClauses()->size();
	
	while (count < iter_count && opt->getHighestScore() != size) {
		cout << ++count;
		opt->runIteration();
	}

	auto end = chrono::system_clock::now();
	auto full_time = chrono::duration_cast<chrono::milliseconds>(end - start);

	cout << "\n\n ------------------------------------------------------------------------------ \n";
	cout << "RUNTIME DURATION  -  " << full_time.count() << "  milliseconds\n";
	cout << "CLAUSES SATISFIED  -  " << opt->getHighestScore() << "\n";
	float score = (float)opt->getHighestScore();
	float percentage = (score / (float)size) * 100;
	cout << "HIGHEST FITNESS ACHIEVED  -  " << percentage << "%\n";

	int* results = opt->getMember(0)->getResultAsTable();
	for (unsigned int i = 0; i < opt->getMember(0)->getSolution()->size(); i++) {
		cout << results[i];
	}
	cout << endl;
	delete results;
	delete opt->getMember(0)->getProblem();
	delete opt;

}

int main() {
	
	string path;
	cout << "Please type in the name of the file, that will be used to run the algorithm" << endl;
	cin >> path;
	
	execute(path);
	cout << endl;

}