#ifndef CentralProcessingUnit_h
#define CentralProcessingUnit_h

#include <stdint.h>
#include <stdlib.h>

#include "Ram.hpp"
#include "ControlUnit.hpp"
#include "ArithmeticalLogicalUnit.hpp"
#include "CommunicationUnit.hpp"

class CentralProcessingUnit
{
private:
	Ram* ram;
	ControlUnit* cu;
	ArithmeticalLogicalUnit* alu;
	CommunicationUnit *comu;

public:
	// constructor:
	CentralProcessingUnit(Ram* ram, int* number_of_calls, int number_of_communication_channels);
	// destructot:
	~CentralProcessingUnit();
	
	// execute cycle:
	bool next_cycle(int debug = 0);
};

#endif
