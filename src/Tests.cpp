#include "Tests.hpp"

Tests::Tests(int debug)
{
	#ifdef DEBUG
		printf("CONSTRUCTOR: %s\n", __PRETTY_FUNCTION__);
	#endif

	vm_is_initialized = false;
	gvm_is_initialized = false;
	this->debug = debug;
}

void Tests::vm_init_random(int size_of_programm)
{
	#ifdef DEBUG
		printf("FUNCTION: %s\n", __PRETTY_FUNCTION__);
	#endif

	program_name = "random";
	// create vm:
	//unsigned int sizeofram = 200;
	vm = new VirtualMachine(size_of_programm);

	// create random program:
	uint8_t* randprog;

	randprog = (uint8_t*)malloc(size_of_programm);
	srand(time(NULL));

	for (int i=0; i<size_of_programm; i++)
		randprog[i] = rand() % NUMBER_OF_INSTRUCTIONS;

	// load programm:
	vm->load_program(randprog, size_of_programm);


	vm_is_initialized = true;
}

void Tests::gvm_init_random(int size_of_programm)
{
	#ifdef DEBUG
		printf("FUNCTION: %s\n", __PRETTY_FUNCTION__);
	#endif

	program_name = "random";
	// create vm:
	//unsigned int sizeofram = 200;
	gvm = new GenericMachine();

	// create random program:
	uint8_t* randprog;

	randprog = (uint8_t*)malloc(size_of_programm);
	srand(time(NULL));

	for (int i=0; i<size_of_programm; i++)
		randprog[i] = rand() % NUMBER_OF_INSTRUCTIONS;

	// load programm:
	gvm->load_program(randprog, size_of_programm);


	gvm_is_initialized = true;
}

void Tests::run_vm()
{
	#ifdef DEBUG
		printf("FUNCTION: %s\n", __PRETTY_FUNCTION__);
	#endif

	if (vm_is_initialized)
	{
		if (debug > 2)
		{
			std::string filename = log_dir+program_name+"_ram_prerun.txt";
			vm->printRAM(filename.c_str());
		}
	
		vm->run(debug);
	
		if (debug > 0)
		{
			std::string filename = log_dir+program_name+"_ram_postrun.txt";
			vm->printRAM(filename.c_str());
		}
	}
	else
		printf("Error! VM not initialized for testing!\n");
}