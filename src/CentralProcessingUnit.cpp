#include "CentralProcessingUnit.hpp"

CentralProcessingUnit::CentralProcessingUnit(int id, Ram * ram, int * number_of_calls, int number_of_communication_channels)
{
	#ifdef DEBUG
		printf("CONSTRUCTOR: %s\n", __PRETTY_FUNCTION__);
	#endif

	this->id = id;
	cycles = 0;

	this->number_of_calls = number_of_calls;
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

	cycles++;
	return cu->next_cycle(debug);
}

void CentralProcessingUnit::print_statistics()
{
	printf("\n### STATISTICS ###\n\n");
	printf(" INSTRUCTION | #CALLS\n");
	printf("---------------------\n");
	for (int i=0; i<NUMBER_OF_INSTRUCTIONS_VM; i++)
	{
		if (INSTRUCTION_PARAMCOUNT[i] >= 0)
			printf("  %#4X (%3s) | %6i\n", i, ASM_SYMBOLS[i].c_str(), number_of_calls[i]);
	}
	printf("---------------------\n");
	printf("       TOTAL | %6i\n", cycles+1);
	printf("\n### END ###\n\n");
}