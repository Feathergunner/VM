#ifndef AbstractMachine_h
#define AbstractMachine_h

#include <vector>

#include "Ram.hpp"
#include "CentralProcessingUnit.hpp"

class AbstractMachine
{
private:
	int number_of_ram_units;
	int number_of_cpu_units;
	
	std::vector<Ram*> ram;
	std::vector<CentralProcessingUnit*> cpu;
	
	// statistics:
	std::vector<int*> number_of_calls;
	std::vector<int> cycles;
	
public:
	// constructors:
	AbstractMachine();
	
	//destructor:
	virtual ~AbstractMachine();
	
	// init:
	virtual void load_program_from_file(const char* filename, int debug) = 0;
	virtual void load_program(uint8_t* program, int length) = 0;
	// print ram
	void printRAM(const char* filename);
	void printRAM(const char* filename, int number_of_ram);
	// start:
	virtual void run(int debug = 0) = 0;
	
	void print_statistics(int number_of_cpu);
};

#endif