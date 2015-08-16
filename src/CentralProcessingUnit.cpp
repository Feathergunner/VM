#include "CentralProcessingUnit.hpp"

CentralProcessingUnit::CentralProcessingUnit(Ram* ram, int* number_of_calls)
{
	this->ram = ram;
	alu = new ArithmeticalLogicalUnit();
	cu = new ControlUnit(alu, ram, number_of_calls);
}

bool CentralProcessingUnit::next_cycle(bool debug)
{
	return cu->next_cycle(debug);
}