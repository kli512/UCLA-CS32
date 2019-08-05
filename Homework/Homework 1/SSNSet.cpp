#include <iostream>
#include "SSNSet.h"
using namespace std;

SSNSet::SSNSet() {}


bool SSNSet::add(ItemType ssn)
{
	return m_ssnset.insert(ssn);
}

int SSNSet::size() const {
	return m_ssnset.size();
}

void SSNSet::print() const {
	for (int i = 0; i < m_ssnset.size(); i++) {
		ItemType j;
		m_ssnset.get(i, j);
		cout << j << endl;
	}
}