#include <stdlib.h>
#include <time.h>

#include "VirtualMachine.hpp"
#include "Tests.hpp"

void example(int debug)
{	
	// sizeofram defines the number of bytes that fit into ram
	int sizeofram = 200;
	
	VirtualMachine* vm = new VirtualMachine(sizeofram);
	
	// load a program from file:
	vm->load_program_from_file("mc_Prim.txt", debug);
	
	if (debug > 2)
		vm->printRAM("logs/before.txt");
	
	vm->run(debug);
	
	if (debug > 0)
		vm->printRAM("logs/ram.txt");
}

int main()
{
	// change this to set the amount of debugging info
	// 0 will display almost nothing
	// 1 will display status in each cycle and save ram after termination to ram.txt
	// 2 will display all of the above, plus a statistic of executed instruction
	// 3 will display all of the above, plus additional description during execution
	int debug = 2;

	Tests t = Tests(debug);
	t.init_random(100);
	t.run_vm();

	//example(debug);
	
	return 0;
}
