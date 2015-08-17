#include "ControlUnit.hpp"

ControlUnit::ControlUnit(ArithmeticalLogicalUnit* alu, Ram* ram, int* number_of_calls)
{
	this->alu = alu;
	this->ram = ram;
	
	this->number_of_calls = number_of_calls;
	
	ic = 0;
}

bool ControlUnit::next_cycle(bool debug)
{
	/*
		FETCH instruction:
	*/
	// only consider lower 4 bits of instruction
	int instruction = ram->get_byte(ic) & 0xFF;
	
	if (debug)
		printf("IC at: %#X, read: %#X\n", ic, instruction);
	
	uint32_t dest, source, value;
	
	/*
		DECODE instruction:
	*/
	switch(instruction)
	{
		/*
		-----------------------------------------------------------------------------------------
		control instructions
		-----------------------------------------------------------------------------------------
		*/
		//#include "Instructions.hpp"
		//printf("stp: %i\n",STP);
		case STP:
			// stop programm
			return false;
			break;
		case JMP:
			// unconditioned jump
			dest = ram->get_int(ic+1);
			// set instruction counter:
			ic = dest;
			if (debug)
				printf("unconditional jump to %#X\n", dest);
			break;
			
		case JGZ:
			// jump if greater than zero:
			if (alu->isGreaterZero())
			{
				// read destination:
				dest = ram->get_int(ic+1);
				// set instruction counter:
				ic = dest;
				if (debug)
					printf("conditional jump to %#X\n", dest);
			} else {
				ic += 1+BYTESIZE_OF_ADRESSSPACE;
				if (debug)
					printf("condition of jump not fulfilled\n");
			}		
			break;
			
		case JOF:
			// jump if overflow:
			if (alu->isOverflow())
			{
				dest = ram->get_int(ic+1);
				
				ic = dest;
				if (debug)
					printf("conditional jump to %#X\n", dest);
			} else {
				ic += 1+BYTESIZE_OF_ADRESSSPACE;
				if (debug)
					printf("condition of jump not fulfilled\n");
			}
			break;
			
		case JEZ:
			// jump if zero:
			if (alu->isZero())
			{
				dest = ram->get_int(ic+1);
				
				ic = dest;
				if (debug)
					printf("conditional jump to %#X\n", dest);
			} else {
				ic += 1+BYTESIZE_OF_ADRESSSPACE;
				if (debug)
					printf("conditional jump to %#X\n", dest);
			}
			
			break;
			
		case JNO:
			// jump if no overflow:
			if (!(alu->isOverflow()))
			{
				dest = ram->get_int(ic+1);
				
				ic = dest;
				if (debug)
					printf("conditional jump to %#X\n", dest);
			} else {
				ic += 1+BYTESIZE_OF_ADRESSSPACE;
				if (debug)
					printf("conditional jump to %#X\n", dest);
			}
			
			break;
			
		/*
		-----------------------------------------------------------------------------------------
			arithmetic instructions
		-----------------------------------------------------------------------------------------
		*/
		case ADD:
			alu->op_add();
			
			ic++;
			
			if (debug)
				printf("add\n");
			break;
			
		case SUB:
			alu->op_sub();
			
			ic++;
			
			if (debug)
				printf("sub\n");
			break;
			
		case MUL:
			alu->op_mul();
			
			ic++;
			
			if (debug)
				printf("mul\n");
				
			break;
			
		case DIV:
			alu->op_div();
			
			ic++;
			
			if (debug)
				printf("div\n");
				
			break;
			
		case AND:
			alu->op_and();
			
			ic++;
			
			if (debug)
				printf("and\n");
			break;
						
		case BOR:
			alu->op_or();
			
			ic++;
			
			if (debug)
				printf("or\n");
			break;
			
		case SHL:
			alu->op_shift_l();
			
			ic++;
			
			if (debug)
				printf("shift_l\n");
			break;
		case SHR:
			alu->op_shift_r();
			
			ic++;
			
			if (debug)
				printf("shift_r\n");
			break;
			
		/*
		-----------------------------------------------------------------------------------------
		data instructions
		-----------------------------------------------------------------------------------------
		*/
		case LDA:
			// load from RAM into register A
			source = ram->get_int(ic+1);
			value = ram->get_int(source);
			
			alu->writeA(value);
			
			ic += 1+BYTESIZE_OF_ADRESSSPACE;
			
			if (debug)
				printf("write %i from ram[%i] to reg_A\n", value, source);
			break;
			
		case LDB:
			// load from RAM into register B
			source = ram->get_int(ic+1);
			value = ram->get_int(source);
			
			alu->writeB(value);
			
			ic += 1+BYTESIZE_OF_ADRESSSPACE;
			
			if (debug)
				printf("write %i from ram[%i] to reg_B\n", value, source);
			break;
			
		case LDC:
			// load constant value into register A
			value = ram->get_int(ic+1);
			
			alu->writeA(value);
			
			ic += 1+BYTESIZE_OF_ADRESSSPACE;
			
			if (debug)
				printf("write %i to reg_A\n", value);
			break;
			
		case LD0:
			// load 0 to register B
			alu->writeB(0);
			
			ic++;
			
			if (debug)
				printf("write 0 to reg_B\n");
			break;
			
		case RLA:
			// load C into A
			alu->writeA(alu->getC());
			
			ic++;
			
			if (debug)
				printf("write C to A\n");
				
			break;
			
		case RLB:
			// load C into B
			alu->writeB(alu->getC());
			
			ic++;
			
			if (debug)
				printf("write C to B\n");
				
			break;
			
		case LDM:
			// load max (0xFFFFFFFF) into B
			alu->writeB(0xFFFFFFFF);
			
			ic++;
			
			if (debug)
				printf("write max to reg_B\n");
			break;
			
		case LD1:
			// load 1 to register B
			alu->writeB(1);
			
			ic++;
			
			if (debug)
				printf("write 1 to reg_B\n");
			break;
			
		case STR:
			// write from register C into RAM
			dest = ram->get_int(ic+1);
			
			value = alu->getC();
			
			ram->store_int(value, dest);
			
			ic += 1+BYTESIZE_OF_ADRESSSPACE;
			
			if (debug)
				printf("store %i (from reg_C) to ram[%i]\n", value, dest);
			break;
			
		case MOV:
			// move data within RAM
			source = ram->get_int(ic+1);
			
			dest = ram->get_int(ic+1+BYTESIZE_OF_ADRESSSPACE);
			
			ram->move(source, dest);
			
			ic += 1 + 2*BYTESIZE_OF_ADRESSSPACE;
			
			if (debug)
				printf("move data in ram from %i to %i\n", source, dest);
			break;
			
		case NOP:
			// no operation
			ic++;
			if (debug)
				printf("NOP\n");
			break;
			
		/*
		-----------------------------------------------------------------------------------------
		default instruction
		-----------------------------------------------------------------------------------------
		*/
		default:
			// NOP
			ic++;
			if (debug)
				printf("unknown instruction. do NOP.\n");
	}
	
	number_of_calls[instruction]++;
	if (debug)
		printf("\ncontrol unit cycle finished\n");
	
	return true;
}
