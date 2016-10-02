#include "VirtualMachine.hpp"

VirtualMachine::VirtualMachine()
{
	#ifdef DEBUG
		printf("CONSTRUCTOR: %s\n", __PRETTY_FUNCTION__);
	#endif
	
	number_of_calls = (int*)malloc(NUMBER_OF_INSTRUCTIONS*4);
	memset(number_of_calls, 0, NUMBER_OF_INSTRUCTIONS*4);
	
	ram = new Ram(0x1000);
	cpu = new CentralProcessingUnit(ram, number_of_calls, 1);
}

VirtualMachine::VirtualMachine(unsigned int ram_size)
{
	#ifdef DEBUG
		printf("CONSTRUCTOR: %s\n", __PRETTY_FUNCTION__);
	#endif
	
	number_of_calls = (int*)malloc(NUMBER_OF_INSTRUCTIONS*4);
	memset(number_of_calls, 0, NUMBER_OF_INSTRUCTIONS*4);

	ram = new Ram(ram_size);
	cpu = new CentralProcessingUnit(ram, number_of_calls, 1);
}

VirtualMachine::~VirtualMachine()
{
	delete &ram;
	delete &cpu;
	
	free(number_of_calls);
}

/*
----------------------------------------------------------------------------------------
	INPUT / INIT
----------------------------------------------------------------------------------------
*/

// reads machine-code program from file given by filename and writes it into RAM,
// beginning at position 0
// file has to start with integer giving the byte-length of the program
void VirtualMachine::load_program_from_file(const char* filename, int debug)
{
	#ifdef DEBUG
		printf("FUNSCTION: %s\n", __PRETTY_FUNCTION__);
	#endif

	int tmp;
	FILE* program;
	
	printf("Loading program...");
	program = fopen(filename,"r");
	
	if (program!=NULL)
	{	
		uint8_t byte;
		int length;
		int size;
		tmp = fscanf(program, "%i ", &length);
		if (debug > 2)
			printf("length is: %i\n\n", length);
		size = length;
		
		for (int i=0; i<size; i++)
		{
			tmp = fscanf(program, "%hhx ", &byte);
			ram->store_byte(byte, i);
			if (debug > 2)
				printf("ram[%3i]=\t %#04x\n", i, ram->get_byte(i));
		}
	}
	else
		printf("Error reading file\n");
}

// loads program passed as byte-array
void VirtualMachine::load_program(uint8_t* program, int length)
{
	#ifdef DEBUG
		printf("FUNCTION: %s\n", __PRETTY_FUNCTION__);
	#endif

	for (int i=0; i<length; i++)
		ram->store_byte(program[i], i);
}

/*
----------------------------------------------------------------------------------------
	RUNNING 
----------------------------------------------------------------------------------------
*/

// main method to start vm.
// debug:
//	= 0	: no debug messages
// 	= 1	: some messages
//	= 2 : all messages
void VirtualMachine::run(int debug)
{
	#ifdef DEBUG
		printf("FUNCTION: %s\n", __PRETTY_FUNCTION__);
	#endif

	printf("Starting Virtual Machine...\n");
	cycles = 0;

	if (debug > 0)
	{
		printf("%8s | %8s | %5s %12s %12s %12s | %5s | %8s %8s %8s\n", "cycle", "ic", "instr", "src", "dest", "val", "FLAGS", "reg A", "reg B", "reg C");
		printf("---------|----------|----------------------------------------------|-------|---------------------------\n");
	}
	while(cpu->next_cycle(debug) && !cpu->is_stopped())
	{
		cycles++;
		if (debug > 2)
			printf("Cycle finished\n");
	}
	printf("Machine terminated!\n");
	
	if (debug > 1)
		print_statistics();
}

void VirtualMachine::stop_machine()
{
	#ifdef DEBUG
		printf("FUNCTION: %s\n", __PRETTY_FUNCTION__);
	#endif

	cpu->stop_cpu();
}

/*
----------------------------------------------------------------------------------------
	DEBUG 
----------------------------------------------------------------------------------------
*/

void VirtualMachine::printRAM(const char* filename)
{
	#ifdef DEBUG
		printf("FUNCTION: %s\n", __PRETTY_FUNCTION__);
	#endif

	ram->print_ram(filename);
}

void VirtualMachine::print_statistics()
{
	#ifdef DEBUG
		printf("FUNCTION: %s\n", __PRETTY_FUNCTION__);
	#endif

	cpu->print_statistics();
}
