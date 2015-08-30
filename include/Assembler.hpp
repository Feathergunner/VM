#ifndef ASSEMBLER_h
#define ASSEMBLER_h

#include <list>
#include <iterator>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>

#include "Instructions_VM.hpp"
#include "Instructions_ASM.hpp"
#include "Constants.hpp"

using namespace std;

class Pair
{
	// pair of id and value to manage pairs of
	// (jump-label, address) and (var-name, address)
	// in preporgram
	
public:
	string name;
	int value;
	
	Pair(string n, int v)
	{
		name = n;
		value = v;
	}
};

class Precode
{
	// stores machine-code command and parameters
public:
	uint8_t code;
	int address;
	int val;
	string param1, param2;
	
	Precode(uint8_t c, int adr, string p1, string p2)
	{
		code = c;
		address = adr;
		param1 = p1;
		param2 = p2;
	}
	
	Precode(uint8_t c, int adr, string p1)
	{
		code = c;
		address = adr;
		param1 = p1;
	}
	
	Precode(uint8_t c, int adr)
	{
		code = c;
		address = adr;
	}
	
	Precode(uint8_t c, int adr, int v)
	{
		code = c;
		address = adr;
		val = v;
	}
};

class Assembler
{
private:
	bool debug;

	const char* filename_input;
	const char* filename_output;
	
	ifstream inputfile;
	
	int number_of_vars;
	int number_of_labels;
	int number_of_commands;
	
	int offset;
	int length_of_header;

	list<Pair> variables;
	list<Pair> labels;
	
	list<Precode> preprogram;
	
	// length in bytes:
	int length_of_code;
	uint8_t* program;
	
	// create precode-code (except jump- and var-labels)
	//int make_preprogram(uint32_t adr_param_1 uint32_t adr_param_2);
	int make_preprogram();
	
	// assemble extended commands:
	void make_eADD(string param1, string param2);
	void make_eSUB(string param1, string param2);
	void make_eMUL(string param1, string param2);
	void make_eDIV(string param1, string param2);
	void make_STC(int value, string var_name);
	
	// update precode jump-labels:
	void update_jump_labels();
	// second iteration, add addresses of jumps and variables,
	// and write code into byte-array:
	int make_program();
	
	// classifies is line is comment, label, code oder var-declaration
	int classify_line(string line_of_code);
	// handle label:
	void add_label(string name_label, int address);
	int find_label_address(string name_label, int* address);
	// handle declaration of variables:
	void add_var(string name_var, int address);
	int find_var_address(string name_var, int* address);
	
	// read parameters:
	int read_int_parameter(std::string::iterator* it, int* i_val);
	int read_string_parameter(std::string::iterator* it, string* s_val);
	
	// write parameter to machine-code program:
	void parse_int(int param, uint8_t* destination);
	
public:
	// constructor:
	Assembler(const char* filename_input, const char* filename_output);
	// destructor:
	~Assembler();
	
	int assemble(int debug = 0);
	// write code to file:
	void write_code();
	
	void print_precode();
	
};

#endif
