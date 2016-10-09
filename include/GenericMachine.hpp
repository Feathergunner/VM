#ifndef GenericMachine_h
#define GenericMachine_h

#include <vector>

#include "Ram.hpp"
#include "CentralProcessingUnit.hpp"

class GenericMachine
{
private:
	int number_of_ram_units;
	int number_of_cpu_units;
	
	std::vector<Ram*> ram;
	std::vector<CentralProcessingUnit*> cpu;
	
	// statistics:
	int cycles;
	
public:
	// constructors:
	GenericMachine(int num_cpu, int num_ram);
	GenericMachine():GenericMachine(1,1){};
	
	//destructor:
	~GenericMachine(){};
	
	// init:
	void load_program_from_file(const char* filename, int debug, int destination_ram = 0);
	void load_program(uint8_t* program, int length, int destination_ram = 0);

	// start:
	void run(int debug = 0);

	// print ram:
	void printRAM(const char* filename, int id_of_ram = 0);
	// print statistics:
	void print_statistics(int id_of_cpu = 0);
};

#endif