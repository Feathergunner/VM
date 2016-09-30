#include "CentralProcessingUnit.hpp"

CentralProcessingUnit::CentralProcessingUnit(Ram* ram, int* number_of_calls, int number_of_communication_channels)
{
	this->ram = ram;
	alu = new ArithmeticalLogicalUnit();
	comu = new CommunicationUnit(number_of_communication_channels);
	cu = new ControlUnit(alu, ram, comu, number_of_calls);
}

CentralProcessingUnit::~CentralProcessingUnit()
{
	delete alu;
	delete cu;
	delete comu;
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
