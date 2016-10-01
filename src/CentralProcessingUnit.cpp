#include "CentralProcessingUnit.hpp"

CentralProcessingUnit::CentralProcessingUnit(Ram * ram, int * number_of_calls, int number_of_communication_channels)
{
	#ifdef DEBUG
		printf("CONSTRUCTOR: %s\n", __PRETTY_FUNCTION__);
	#endif

	this->ram = ram;
	alu = new ArithmeticalLogicalUnit();
	comu = new CommunicationUnit(number_of_communication_channels);
	cu = new ControlUnit(alu, ram, comu, number_of_calls);
}

CentralProcessingUnit::~CentralProcessingUnit()
{
	#ifdef DEBUG
		printf("DESTRUCTOR: %s\n", __PRETTY_FUNCTION__);
	#endif

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
	#ifdef DEBUG
		printf("FUNCTION: %s\n", __PRETTY_FUNCTION__);
	#endif

	return cu->next_cycle(debug);
}
