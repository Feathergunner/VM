#ifndef ControlUnit_h
#define ControlUnit_h

#include <stdint.h>
#include <stdio.h>
#include <ctime>

#include "Ram.hpp"
#include "ArithmeticalLogicalUnit.hpp"
#include "Instructions_VM.hpp"

class ControlUnit
{
private:
	// instruction counter:
	uint16_t ic;
	
	// pointer to ram:
	Ram* ram;
	// pointer to alu:
	ArithmeticalLogicalUnit* alu;
	
	// measuring:
	int* number_of_calls;
public:
	// constructor:
	ControlUnit(ArithmeticalLogicalUnit* alu, Ram* ram, int* number_of_calls);
	// destructor:
	~ControlUnit(){}
	
	// execute cycle:
	bool next_cycle(bool debug = false);
	
	
};

#endif
