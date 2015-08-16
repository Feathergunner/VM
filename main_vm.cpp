#include <stdlib.h>
#include <time.h>

#include "VirtualMachine.hpp"

int main()
{
	VirtualMachine* vm = new VirtualMachine(300);
	
	// load a program:
	vm->load_program_from_file("mc_test.txt");
	
	/*
	// create random program:
	uint8_t* randprog;
	int progsize = 256;
	randprog = (uint8_t*)malloc(progsize);
	srand(time(NULL));
	for (int i=0; i<progsize; i++)
		randprog[i] = rand() % 256;
	vm->load_program(randprog, progsize);
	vm->printRAM("before.txt");
	*/
	
	// run:
	vm->run();
	// see memory:
	vm->printRAM("ram.txt");
	vm->print_statistics();
	return 0;
}
