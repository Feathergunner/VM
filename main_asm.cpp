#include "Assembler.hpp"

int main()
{
	///*
	const char* filename_input = "asm_test.txt";
	const char* filename_output = "mc_test.txt";
	//*/
	/*
	const char* filename_input = "asm_multiply_test.txt";
	const char* filename_output = "mc_multiply_test.txt";
	*/
	/*
	const char* filename_input = "../programms/asm_test.txt";
	const char* filename_output = "mc_test.txt";
	*/
	
	Assembler assm(filename_input, filename_output);
	int err = assm.assemble(true);
	assm.print_precode();
	assm.write_code();

	return 0;
}
