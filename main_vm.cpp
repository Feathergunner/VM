#include <stdlib.h>
#include <time.h>

#include "VirtualMachine.hpp"

int main()
{
	// change this to set the amount of debugging info
	// 0 will display almost nothing
	// 1 will display status in each cycle and save ram after termination to ram.txt
	// 2 will display all of the above, plus a statistic of executed instruction
	// 3 will display all of the above, plus additional description during execution
	int debug = 2;
	int sizeofram = 200;
	
	VirtualMachine* vm = new VirtualMachine(sizeofram);
	
	// use one of the following options:
	// 1) load program from file
	// 2) create random program
	
	// 1) load a program:
	vm->load_program_from_file("mc_Prim.txt", debug);
	
	/*
	// 2) create random program:
	uint8_t* randprog;
	int progsize = 30;
	randprog = (uint8_t*)malloc(progsize);
	srand(time(NULL));
	for (int i=0; i<progsize; i++)
		randprog[i] = rand() % NUMBER_OF_INSTRUCTIONS;
	vm->load_program(randprog, progsize);
	*/
	
	if (debug > 2)
		vm->printRAM("before.txt");
	
	vm->run(debug);
	
	if (debug > 0)
		vm->printRAM("ram.txt");
	return 0;
}
