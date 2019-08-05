#ifndef SSNSET_INCLUDED
#define SSNSET_INCLUDED

#include "Set.h"

class SSNSet
{
public:
	SSNSet();		// Create an empty SSN set.

	bool add(ItemType ssn);
	// Add an SSN to the SSNSet. Return true if and only if the SSN
	// was actually added.

	int size() const;	// Return the number of SSNs in the SSNSet.

	void print() const;
	// Write every SSN in the SSNSet to cout exactly once, one per
	// line. Write no other text.

private:
	// Some of your code goes here.
	Set m_ssnset;
};
#endif	// SSNSet included