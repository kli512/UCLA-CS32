#include <iostream>
#include <cassert>

#include "SSNSet.h"

using namespace std;

int main() {
	
	SSNSet ss;
	ss.add(12345);
	ss.add(1234);
	assert(ss.add(1234) == false);
	ss.add(12345);
	ss.add(12345);
	ss.add(123456);
	ss.add(1234567);
	ss.add(12345);
	assert(ss.size() == 4);

	cout << "assertions successful" << endl;
	ss.print();
}