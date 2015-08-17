#ifndef CentralProcessingUnit_h
#define CentralProcessingUnit_h

#include <stdint.h>
#include <stdlib.h>

#include "Ram.hpp"
#include "ControlUnit.hpp"
#include "ArithmeticalLogicalUnit.hpp"

class CentralProcessingUnit
{
private:
	Ram* ram;
	ControlUnit* cu;
	ArithmeticalLogicalUnit* alu;

public:
	// constructor:
	CentralProcessingUnit(Ram* ram, int* number_of_calls);
	// destructot:
	~CentralProcessingUnit(){}
	
	// execute cycle:
	bool next_cycle(int debug = 0);
};

#endif
