#include <stdlib.h>
#include <time.h>

#include "VirtualMachine.hpp"

int main()
{
	VirtualMachine* vm = new VirtualMachine(300);
	
	// load a program:
	//vm->load_program_from_file("mc_test.txt");
	
	///*
	// create random program:
	uint8_t* randprog;
	int progsize = 256;
	randprog = (uint8_t*)malloc(progsize);
	srand(time(NULL));
	for (int i=0; i<progsize; i++)
		randprog[i] = rand() % 256;
	vm->load_program(randprog, progsize);
	vm->printRAM("before.txt");
	//*/
	
	// change this to set the amount of debugging info
	// 0 will display almost nothing
	// 1 will display status in each cycle and save ram after termination to ram.txt
	// 2 will display additional informations while executing vm
	int debug = 1;
	
	vm->run(debug);
	
	if (debug > 0)
		vm->printRAM("ram.txt");
	if (debug > 1)
		vm->print_statistics();
	return 0;
}
