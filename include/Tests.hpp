#ifndef Tests_h
#define Tests_h

#include <stdlib.h>
#include <string.h>
#include <vector>

#include "VirtualMachine.hpp"
#include "GenericMachine.hpp"
#include "Constants.hpp"

class Tests
{
private:
	VirtualMachine *vm;
	GenericMachine *gvm;
	bool vm_is_initialized;
	bool gvm_is_initialized;
	int debug;
	std::string program_name;

	const char* machine_dir = "machines/";
	const char* log_dir = "logs/";

public:
	Tests(int debug=3);
	~Tests(){};
	void vm_init_random(int size_of_programm);
	void gvm_init_random(int size_of_programm);
	void run_vm();
	void run_gvm();
};

#endif