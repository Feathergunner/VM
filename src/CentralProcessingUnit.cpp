#include "CentralProcessingUnit.hpp"

CentralProcessingUnit::CentralProcessingUnit(Ram* ram, int* number_of_calls)
{
	this->ram = ram;
	alu = new ArithmeticalLogicalUnit();
	cu = new ControlUnit(alu, ram, number_of_calls);
}

// method to execute the next cycle
// debug:
//	= 0	: no debug messages
// 	= 1	: some messages
//	= 2 : all messages
bool CentralProcessingUnit::next_cycle(int debug)
{
	return cu->next_cycle(debug);
}
