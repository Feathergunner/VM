#ifndef VirtualMachine_h
#define VirtualMachine_h

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "Ram.hpp"
#include "ControlUnit.hpp"
#include "ArithmeticalLogicalUnit.hpp"
#include "CentralProcessingUnit.hpp"

#include "Constants.hpp"

class VirtualMachine
{
private:
	Ram* ram;	
	CentralProcessingUnit* cpu;
	
	// statistics measurement:
	int* number_of_calls;
	int cycles;
	bool stop;

public:
	// constructors:
	VirtualMachine(unsigned int ram_size);
	VirtualMachine();
	// destructor:
	~VirtualMachine();
	
	// init:
	void load_program_from_file(const char* filename, int debug);
	void load_program(uint8_t* program, int length);
	// print ram
	void printRAM(const char* filename);
	// start:
	void run(int debug = 0);
	// stop:
	void stop_machine();
	
	void print_statistics();
	
};

#endif
