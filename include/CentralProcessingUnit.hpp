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
	int id;
	int cycles;
	int* number_of_calls;

	Ram * ram;
	ArithmeticalLogicalUnit * alu;
	CommunicationUnit *	comu;
	ControlUnit * cu;

public:
	// constructors:
	CentralProcessingUnit(int id, Ram * ram, int * number_of_calls, int number_of_communication_channels);
	CentralProcessingUnit(Ram * ram, int * number_of_calls, int number_of_communication_channels):
		CentralProcessingUnit(0, ram, number_of_calls, number_of_communication_channels){};
	// destructot:
	~CentralProcessingUnit();
	
	// execute cycle:
	bool next_cycle(int debug = 0);

	// print statistics of this cpu:
	void print_statistics();
};

#endif
