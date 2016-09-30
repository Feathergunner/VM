#include "ControlUnit.hpp"

ControlUnit::ControlUnit(ArithmeticalLogicalUnit* alu, Ram* ram, int* number_of_calls)
{
	this->alu = alu;
	this->ram = ram;
	
	this->number_of_calls = number_of_calls;
	
	ic = 0;
	cc = 0;
	//ins = 0;
	
	func[0x00] = &ControlUnit::func_NOP;
	func[0x01] = &ControlUnit::func_JMP;
	func[0x02] = &ControlUnit::func_JGZ;
	func[0x03] = &ControlUnit::func_JOF;
	
	func[0x04] = &ControlUnit::func_ADD;
	func[0x05] = &ControlUnit::func_SUB;
	func[0x06] = &ControlUnit::func_AND;
	func[0x07] = &ControlUnit::func_BOR;
	func[0x08] = &ControlUnit::func_SHL;
	func[0x09] = &ControlUnit::func_SHR;
	func[0x0A] = &ControlUnit::func_LDA;
	func[0x0B] = &ControlUnit::func_LDB;
	func[0x0C] = &ControlUnit::func_LDC;
	func[0x0D] = &ControlUnit::func_LD0;
	
	func[0x0E] = &ControlUnit::func_STR;
	func[0x0F] = &ControlUnit::func_MOV;
	
	func[0x10] = &ControlUnit::func_NOP;
	func[0x11] = &ControlUnit::func_NOP;
	func[0x12] = &ControlUnit::func_JEZ;
	func[0x13] = &ControlUnit::func_JNO;
	func[0x14] = &ControlUnit::func_MUL;
	func[0x15] = &ControlUnit::func_DIV;
	func[0x16] = &ControlUnit::func_NOP;
	func[0x17] = &ControlUnit::func_NOP;
	func[0x18] = &ControlUnit::func_NOP;
	func[0x19] = &ControlUnit::func_NOP;
	func[0x1A] = &ControlUnit::func_RLA;
	func[0x1B] = &ControlUnit::func_RLB;
	func[0x1C] = &ControlUnit::func_LDM;
	func[0x1D] = &ControlUnit::func_LD1;

	func[0x20] = &ControlUnit::func_SND;
	func[0x21] = &ControlUnit::func_RCV;
	func[0x22] = &ControlUnit::func_JCF;
}

// method to execute the next cycle
// debug:
//	= 0	: no debug messages
// 	= 1	: some messages
//	= 2 : all messages
bool ControlUnit::next_cycle(int debug)
{
	/*
		INIT:
	*/
	this->debug = debug;
	
	cc++;
	
	old_ic = ram->check_address(ic);
	
	source = -1;
	dest = -1;
	value = -1;
	
	/*
		FETCH instruction:
	*/
	int instruction = (ram->get_byte(ic)) & 0x1F;
	
	number_of_calls[instruction]++;
	
	if (debug > 2)
		printf("\tread byte: %#2X\n", instruction);
	
	/*
		DECODE instruction:
	*/
		
	if (instruction == STP)
	{
		if (debug > 0)
			print_vm_status(instruction);
		if (debug > 2)
			printf("\ncontrol unit cycle finished\n");
		return false;
	}
	
	(this->*func[instruction])();
	
	if (debug > 0)
		print_vm_status(instruction);
	
	if (debug > 2)
		printf("\ncontrol unit cycle finished\n");
	
	return true;
}

/*
-----------------------------------------------------------------------------------------
	INSTRUCTIONS
-----------------------------------------------------------------------------------------
*/
/*
-----------------------------------------------------------------------------------------
control instructions
-----------------------------------------------------------------------------------------
*/

// unconditioned jump
void ControlUnit::func_JMP()
{
	dest = ram->get_int(ic+1);
	ic = dest;
	
	if (debug > 2)
		printf("unconditional jump to %#X\n", ic);
}


// jump if greater than zero:
void ControlUnit::func_JGZ()
{
	dest = ram->get_int(ic+1);
	if (alu->isGreaterZero())
	{
		ic = dest;
		
		if (debug > 2)
			printf("conditional jump to %#X\n", ic);
	} else {
		ic += 1+WORDSIZE;
		
		if (debug > 2)
			printf("condition of jump not fulfilled\n");
	}		
}

// jump if overflow:
void ControlUnit::func_JOF()
{
	dest = ram->get_int(ic+1);
	if (alu->isOverflow())
	{
		ic = dest;
		
		if (debug > 2)
			printf("conditional jump to %#X\n", ic);
	} else {
		ic += 1+WORDSIZE;
	
		if (debug > 2)
			printf("condition of jump not fulfilled\n");
	}
}

// jump if zero:
void ControlUnit::func_JEZ()
{
	dest = ram->get_int(ic+1);
	if (alu->isZero())
	{
		ic = dest;

		if (debug > 2)
			printf("conditional jump to %#X\n", ic);
	} else {
		ic += 1+WORDSIZE;
		
		if (debug > 2)
			printf("condition of jump not fulfilled\n");
	}
}

// jump if no overflow:
void ControlUnit::func_JNO()
{
	dest = ram->get_int(ic+1);
	if (!(alu->isOverflow()))
	{
		ic = dest;

		if (debug > 2)
			printf("conditional jump to %#X\n", ic);
	} else {
		ic += 1+WORDSIZE;

		if (debug > 2)
			printf("condition of jump not fulfilled\n");
	}
}

// no operation
void ControlUnit::func_NOP()
{
	ic++;
}

/*
-----------------------------------------------------------------------------------------
	arithmetic instructions
-----------------------------------------------------------------------------------------
*/

// addition
void ControlUnit::func_ADD()
{
	alu->op_add();			
	ic++;
}

// substraction
void ControlUnit::func_SUB()
{
	alu->op_sub();	
	ic++;
}

void ControlUnit::func_AND()
{
	alu->op_and();		
	ic++;
}

void ControlUnit::func_BOR()
{
	alu->op_or();		
	ic++;
}

void ControlUnit::func_SHL()
{
	alu->op_shift_l();
	ic++;
}

void ControlUnit::func_SHR()
{
	alu->op_shift_r();
	ic++;
}

void ControlUnit::func_MUL()
{
	alu->op_mul();			
	ic++;		
}

void ControlUnit::func_DIV()
{
	alu->op_div();			
	ic++;
}

/*
-----------------------------------------------------------------------------------------
data instructions
-----------------------------------------------------------------------------------------
*/

// load from RAM into register A
void ControlUnit::func_LDA()
{
	source = ram->get_int(ic+1);
	value = ram->get_int(source);
			
	alu->writeA(value);
		
	ic += 1+WORDSIZE;
}

// load from RAM into register B
void ControlUnit::func_LDB()
{
	source = ram->get_int(ic+1);
	value = ram->get_int(source);
	alu->writeB(value);
		
	ic += 1+WORDSIZE;
}

// load constant value into register A
void ControlUnit::func_LDC()
{
	value = ram->get_int(ic+1);
	alu->writeA(value);
			
	ic += 1+WORDSIZE;
}

// load 0 to register B
void ControlUnit::func_LD0()
{
	alu->writeB(0);
	ic++;
}

void ControlUnit::func_LD1()
{
	alu->writeB(1);
	ic++;
}

// write from register C into RAM
void ControlUnit::func_STR()
{
	dest = ram->get_int(ic+1);
			
	value = alu->getC();
			
	ram->store_int(value, dest);
			
	ic += 1+WORDSIZE;
}

// move data within RAM
void ControlUnit::func_MOV()
{
	source = ram->get_int(ic+1);
	dest = ram->get_int(ic+1+WORDSIZE);
			
	ram->move(source, dest);
			
	ic += 1 + 2*WORDSIZE;
}

// load from reg_C into reg_A
void ControlUnit::func_RLA()
{
	alu->writeA(alu->getC());
	ic++;				
}

// load from reg_C into reg_B
void ControlUnit::func_RLB()
{
	alu->writeB(alu->getC());
	ic++;
}

// load max (0xFFFFFFFF) into B
void ControlUnit::func_LDM()
{
	alu->writeB(0xFFFFFFFF);	
	ic++;
}

/*
-----------------------------------------------------------------------------------------
communication control instructions
-----------------------------------------------------------------------------------------
*/
// write to CommunicationChannel
void ControlUnit::func_SND()
{
	channel_id = ram->get_byte(ic+1);
	
	// TO DO:
	// add check if channel exists
	bool succ;
	comu->provide_value(alu->getC(), channel_id, &succ);
	if (succ)
		ic += 2;
}

// read from CommunicationChannel
void ControlUnit::func_RCV()
{
	channel_id = ram->get_byte(ic+1);
	
	// TO DO:
	// add check if channel exists
	bool succ;
	uint8_t val = comu->get_extern_value(channel_id, &succ);
	if (succ)
	{
		alu->writeA(val);
		ic += 2;
	}
}

// jump if CommunicationChannel is free
void ControlUnit::func_JCF()
{
	channel_id = ram->get_byte(ic+1);
	dest = ram->get_int(ic+2);
	if (comu->check_lock_status(channel_id))
	{
		ic = 2+WORDSIZE;
	}
	else
	{
		ic += dest;
	}
}

/*
----------------------------------------------------------------------------
	Method to display status of VM in each cycle
----------------------------------------------------------------------------
*/

void ControlUnit::print_vm_status(int instr)
{
	// Instruction:
	printf("%8i | %8i | %5s ", cc, old_ic, ASM_SYMBOLS[instr].c_str());
	
	// Adresses & Values:
	if (source != 0xFFFFFFFF)
		printf("%#12X ", ram->check_address(source));
	else
		printf("%12s ", "-");
	
	if (dest != 0xFFFFFFFF)
		printf("%#12X ", ram->check_address(dest));
	else
		printf("%12s ", "-");
		
	if (value != 0xFFFFFFFF)
		printf("%#12X ", value);
	else
		printf("%12s ", "-");
		
	printf("|  ");
		
	// Flags:
	if (alu->isZero())
		printf("Z");
	else
		printf("-");
		
	if (alu->isGreaterZero())
		printf("G");
	else
		printf("-");
		
	if (alu->isOverflow())
		printf("O");
	else
		printf("-");
		
	if (alu->isDivideByZero())
		printf("D");
	else
		printf("-");
		
	// Registers:
	printf(" | %8i %8i %8i", alu->getA(), alu->getB(), alu->getC());
	
		
	printf("\n");
		
}
