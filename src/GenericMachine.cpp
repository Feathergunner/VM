#include "GenericMachine.hpp"

GenericMachine::GenericMachine(int num_cpu, int num_ram)
{
	#ifdef DEBUG
		printf("CONSTRUCTOR: %s\n", __PRETTY_FUNCTION__);
	#endif

	number_of_ram_units = num_ram;
	number_of_cpu_units = num_cpu;

	ram = std::vector<Ram*>(number_of_ram_units);
	cpu = std::vector<CentralProcessingUnit*>(number_of_cpu_units);

	cycles = 0;
}

void GenericMachine::load_program_from_file(const char* filename, int debug, int destination_ram)
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
			ram[destination_ram]->store_byte(byte, i);
		}
	}
	else
		printf("Error reading file\n");
}

void GenericMachine::load_program(uint8_t* program, int length, int destination_ram)
{
	#ifdef DEBUG
		printf("FUNSCTION: %s\n", __PRETTY_FUNCTION__);
	#endif

	for (int i=0; i<length; i++)
		ram[destination_ram]->store_byte(program[i], i);
}

void GenericMachine::run(int debug)
{
	#ifdef DEBUG
		printf("FUNSCTION: %s\n", __PRETTY_FUNCTION__);
	#endif

	cycles = 0;
	bool something_still_running = true;

	while(something_still_running)
	{
		cycles++;
		something_still_running = false;
		for (int i=0; i<number_of_cpu_units; ++i)
		{
			if (!cpu[i]->is_stopped())
			{
				something_still_running = true;
				cpu[i]->next_cycle(debug);
			}
		}
		if (debug > 2)
			printf("Cycle finished\n");
	}
	printf("Machine terminated!\n");

	if (debug > 1)
		print_statistics();
}

void GenericMachine::printRAM(const char* filename, int id_of_ram)
{
	if (id_of_ram < number_of_ram_units)
	{
		ram[id_of_ram]->print_ram(filename);
	}
}

void GenericMachine::print_statistics(int id_of_cpu)
{
	if (id_of_cpu < number_of_cpu_units)
	{
		cpu[id_of_cpu]->print_statistics();
	}

}