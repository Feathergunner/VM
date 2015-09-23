#ifndef ControlUnit_h
#define ControlUnit_h

#include <stdint.h>
#include <stdio.h>
#include <ctime>
#include "Ram.hpp"
#include "ArithmeticalLogicalUnit.hpp"
#include "Instructions_VM.hpp"

#include "Constants.hpp"

class ControlUnit
{
private:
	// instruction counter:
	uint32_t old_ic;
	uint32_t ic;
	// cycle counter:
	uint32_t cc;
	
	// pointer to ram:
	Ram* ram;
	// pointer to alu:
	ArithmeticalLogicalUnit* alu;
	// pointer to CommunicationUnit:
	CommunicationUnit* comu;
	
	// cpu functionalities:
	// temp values:
	uint32_t dest, source, value;
	uint8_t channel_id;
	
	void func_JMP();
	void func_JGZ();
	void func_JOF();
	
	void func_ADD();
	void func_SUB();
	void func_AND();
	void func_BOR();
	void func_SHL();
	void func_SHR();
	
	void func_LDA();
	void func_LDB();
	void func_LDC();
	void func_LD0();
	
	void func_STR();
	void func_MOV();
	
	void func_NOP();
	
	void func_JEZ();
	void func_JNO();
	
	void func_MUL();
	void func_DIV();
	
	void func_RLA();
	void func_RLB();
	
	void func_LDM();
	void func_LD1();
	
	void func_SND();
	void func_RCV();
	
	void (ControlUnit::*func[0x20])();
	
	// measuring / debugging:
	int debug;
	
	int* number_of_calls;
	
	void print_vm_status(int instr);
public:
	// constructor:
	ControlUnit(ArithmeticalLogicalUnit* alu, Ram* ram, int* number_of_calls);
	// destructor:
	~ControlUnit(){}
	
	// execute cycle:
	bool next_cycle(int debug = 0);
};


#endif
