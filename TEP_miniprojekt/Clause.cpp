#include "Clause.h"
#include "CIndividual.h"

bool Clause::isSatisfied(CIndividual* current) {
	
	int temp = 0;
	for (unsigned int i = 0; i < signatures.size(); i++) {
		if (signatures[i] >= 0) {
			temp = current->getSolution()->at(abs(signatures[i]));
		}
		else {
			temp = current->getSolution()->at(abs(signatures[i])) * (-1);
		}

		if (temp == 1) {
			return true;
		}
	}
	return false;
	
}

std::vector< int >* Clause::getSignatures() { return &signatures; }
