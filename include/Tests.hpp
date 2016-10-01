#ifndef Tests_h
#define Tests_h

#include <stdlib.h>
#include <string.h>
#include <vector>

#include "VirtualMachine.hpp"
#include "Constants.hpp"

class Tests
{
private:
	VirtualMachine *vm;
	bool is_initialized;
	int debug;
	std::string program_name;

	const char* machine_dir = "machines/";
	const char* log_dir = "logs/";

public:
	Tests(int debug=3);
	~Tests(){};
	void init_random(int size_of_programm);
	void run_vm();
};

#endif