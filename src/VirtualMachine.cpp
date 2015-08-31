#include "VirtualMachine.hpp"

VirtualMachine::VirtualMachine()
{
	number_of_calls = (int*)malloc(NUMBER_OF_INSTRUCTIONS*4);
	memset(number_of_calls, 0, NUMBER_OF_INSTRUCTIONS*4);
	
	ram = new Ram(0x1000);	
	cpu = new CentralProcessingUnit(ram, number_of_calls);
}

VirtualMachine::VirtualMachine(unsigned int ram_size)
{
	number_of_calls = (int*)malloc(NUMBER_OF_INSTRUCTIONS*4);
	memset(number_of_calls, 0, NUMBER_OF_INSTRUCTIONS*4);
	
	ram = new Ram(ram_size);	
	cpu = new CentralProcessingUnit(ram, number_of_calls);
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
	FILE* program;
	
	printf("Loading program...");
	program = fopen(filename,"r");
	
	if (program!=NULL)
	{	
		uint8_t byte;
		int length;
		int size;
		fscanf(program, "%i ", &length);
		if (debug > 1)
			printf("length is: %i\n\n", length);
		size = length;
		
		for (int i=0; i<size; i++)
		{
			fscanf(program, "%hhx ", &byte);
			ram->store_byte(byte, i);
			if (debug > 1)
				printf("ram[%3i]=\t %#04x\n", i, ram->get_byte(i));
		}
	}
	else
		printf("Error reading file\n");
}

// loads program passed as byte-array
void VirtualMachine::load_program(uint8_t* program, int length)
{
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
	printf("Starting Virtual Machine...\n");
	cycles = 0;
	if (debug == 1)
	{
		printf("%8s | %8s | %5s %12s %12s %12s | %5s\n", "cycle", "ic", "instr", "src", "dest", "val", "FLAGS");
		printf("---------|----------|----------------------------------------------|-------\n");
	}
	while(cpu->next_cycle(debug))
	{
		cycles++;
		if (debug == 2)
			printf("Cycle finished\n");
	}
	printf("Machine terminated!\n");
}

/*
----------------------------------------------------------------------------------------
	DEBUG 
----------------------------------------------------------------------------------------
*/

void VirtualMachine::printRAM(const char* filename)
{
	ram->print_ram(filename);
}

void VirtualMachine::print_statistics()
{
	for (int i=0; i<NUMBER_OF_INSTRUCTIONS; i++)
	{
		if (INSTRUCTION_PARAMCOUNT[i] >= 0)
			printf("instruction %#X:\n\tnumber of calls: %i\n\n", i, number_of_calls[i]);
	}
	printf("Number of processor cycles: %i\n", cycles);
}
