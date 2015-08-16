#include "Assembler.hpp"

Assembler::Assembler(const char* filename_input, const char* filename_output)
{	
	this->filename_input = filename_input;
	this->filename_output = filename_output;
	
	// init counters of variables and labels:
	number_of_vars = 0;
	number_of_labels = 0;
	number_of_commands = 0;
	
	// init length:
	length_of_code = 0;
}

Assembler::~Assembler()
{
	free(program);
}

/*
----------------------------------------------------------------------------------------
	MAIN FUNCTIONALITIES
----------------------------------------------------------------------------------------
*/

// first iteration:
//	- decode commands,
//	- create lists of variables and labels
//	- compute addresses
int Assembler::make_preprogram()
{
	// returns error code if error
	/*
		0	: success
		1	: error while reading command
		2	: error while reading var
	*/
	
	if (debug)
		printf("\nFUNCTION: make_preprogram\n");
	
	// set filestream:
	inputfile.open(filename_input, ifstream::in);

	int maxchars = 60;
	char line_of_code[maxchars];
	
	// add initial jump to begin
	string label_begin("BEGIN");
	preprogram.push_back(Precode(0x1, length_of_code, label_begin));
	number_of_commands++;
	// length_of_code += 5;
	
	// read from file:
	inputfile.getline(line_of_code, maxchars);
	int line_number = 1;
	offset = 0;
	
	while(inputfile.good())
	{
		if (debug)
			printf("\nVAR: line_of_code = %i %s\n", line_number, line_of_code);
		
		int type = classify_line(line_of_code);
		if (debug)
			printf("Var: type         = %i\n",type);
		
		// check if error
		if (type == 0x100)
			return 1; // error
		
		// label
		if (type == 0x102)
		{
			// add label to next position in memory
			// get name:
			string name_label;
			read_single_string_parameter(line_of_code, &name_label);
			add_label(name_label, offset);
		}
		
		// var
		if (type == 0x103)
		{
			// declaration of variable
			// get name:
			string name_var;
			read_single_string_parameter(line_of_code, &name_var);
			// add var:
			add_var(name_var, 5 + 4*number_of_vars);
			
		}
			
		// begin-label
		if (type == 0x104)
			add_label(label_begin, offset);
			
		if (type < 0x100)
		{
			number_of_commands++;
			
			int err, val1, val2;
			string p1, p2;
			
			// if code has parameters:
			if (type == JMP || type == JGZ || type == JOF || type == JEZ || type == JNO || type == LDA || type == LDB || type == STR || type == MOV)
			{
				// cases with variable-name or jump-label
				// read one parameter:
				err = read_single_string_parameter(line_of_code, &p1);
				// if no error occured, set precode:
				if (err == 0)
					preprogram.push_back(Precode(type, offset, p1));
				else
					return 2;
					
				offset += 5;
			}
			
			if (type == LDC)
			{
				// case load_constant: read integer:
				// read one integer parameter:
				err = read_single_int_parameter(line_of_code, &val1);
				// if no error occured, set precode:
				if (err == 0)
					preprogram.push_back(Precode(type, offset, val1));
				else
					return 2;
					
				offset += 5;
			}
			
			if (type == MOV) 
			{
				// case move: to variable-names
				// read two parameters:
				err = read_double_string_parameter(line_of_code, &p1, &p2);
				// if no error occured, set precode:
				if (err == 0)
					preprogram.push_back(Precode(type, offset, p1, p2));
				else
					return 2;
			
				offset += 9;
			}	
			
			if (type == ADD || type == SUB || type == AND || type == BOR || type == MUL || type == DIV || type == SHL || type == SHR || type == LD0 || type == LDM || type == LD1 || type == RLA || type == RLB || type == NOP || type == STP)
			{
				// no parameters:
				preprogram.push_back(Precode(type, offset));
				
				offset += 1;
			}
			
			// extended instructions:
			if (type == EAD || type == ESU || type == EMU || type == EDI)
			{
				// get operands:
				err = read_double_string_parameter(line_of_code, &p1, &p2);
				if (err == 0)
				{
					if (type == EAD)
						make_eADD(p1, p2);
					
					if (type == ESU)
						make_eSUB(p1, p2);
					
					if (type == EMU)
						make_eMUL(p1, p2);
					
					if (type == EDI)
						make_eDIV(p1,p2);
				} else
					return 2;
			}
			//if (type == STC)
			
		}
		
		inputfile.getline(line_of_code, maxchars);
		line_number++;
	}
	
	inputfile.close();
	
	// length of header (initial jump + all vars)
	// i.e. the basis for all precomputed adress-offsets:
	length_of_header = 5 + 4*number_of_vars;

	length_of_code = length_of_header + offset;
	
	update_jump_labels();
	
	return 0;
}

// update jump-labels after first iteration, when all adresses are known
void Assembler::update_jump_labels()
{
	for (list<Pair>::iterator it = labels.begin(); it != labels.end(); it++)
		it->value += length_of_header;
}

void Assembler::make_eADD(string param1, string param2)
{
	preprogram.push_back(Precode(LDA, offset, param1));
	offset += 5;
	preprogram.push_back(Precode(LDB, offset, param2));
	offset += 5;
	preprogram.push_back(Precode(ADD, offset));
	offset += 1;
}

void Assembler::make_eSUB(string param1, string param2)
{
	preprogram.push_back(Precode(LDA, offset, param1));
	offset += 5;
	preprogram.push_back(Precode(LDB, offset, param2));
	offset += 5;
	preprogram.push_back(Precode(SUB, offset));
	offset += 1;
}

void Assembler::make_eMUL(string param1, string param2)
{
	preprogram.push_back(Precode(LDA, offset, param1));
	offset += 5;
	preprogram.push_back(Precode(LDB, offset, param2));
	offset += 5;
	preprogram.push_back(Precode(MUL, offset));
	offset += 1;
}

void Assembler::make_eDIV(string param1, string param2)
{
	preprogram.push_back(Precode(LDA, offset, param1));
	offset += 5;
	preprogram.push_back(Precode(LDB, offset, param2));
	offset += 5;
	preprogram.push_back(Precode(DIV, offset));
	offset += 1;
}

// second iteration:
//	- insert correct addresses for variables and jump-addresses
int Assembler::make_program()
{	
	if (debug)
	{
		printf("\nFUNCTION: make_program\n");
		printf("\tinitialize %i bytes for program\n",length_of_code);
	}
	
	// create byte-array for program:
	program = (uint8_t*)malloc(length_of_code);
	memset(program, 0, length_of_code);
	
	// initialize byte-counter:
	int c = 0;
	
	// iterate through preprogram:
	for (list<Precode>::iterator pc_iterator = preprogram.begin(); pc_iterator != preprogram.end(); pc_iterator++)
	{
		if (debug)
			printf("STATUS: reading next precode\n");
			
		// get current code:
		uint8_t code = pc_iterator->code;
		
		if (pc_iterator != preprogram.begin())
		{
			program[pc_iterator->address + length_of_header] = code;
			c = pc_iterator->address + length_of_header + 1;
		}
		else
		{
			program[pc_iterator->address] = code;
			c = pc_iterator->address+ 1;
		}
		if (debug)
			printf("VAR: code at byte %i: %i\n",pc_iterator->address,code);
		
		//check if current command is command with parameters and add them to program:
		int value, err = -1;
		
		// case jump-label:
		if (code == JMP || code == JGZ || code == JOF || code == JEZ || code == JNO)
		{
			if (debug)
				printf("CASE: jump - get address\n");
			err = find_label_address(pc_iterator->param1, &value);
		}
		
		// case var-name:
		if (code == LDA || code == LDB || code == STR || code == MOV)
		{
			if (debug)
				printf("CASE: variable - get address\n");
			err = find_var_address(pc_iterator->param1, &value);
		}
		
		// case constant value:
		if (code == LDC)
		{
			if (debug)
				printf("Case: constant value\n");
			value = pc_iterator->val;
			// set err to 0 to flag for parsing:
			err = 0;
		}
			
		// if no error occured:
		if (err == 0)
		{
			parse_int(value, &program[c]);
			c += 4;
		}
		else if (err != -1)
			return 1;
			
		err = -1;
			
		// check if current command has second parameter, and add it to program:
		// case MOV:
		if (code == MOV)
		{
			if (debug)
				printf("Case: move - get second address\n");
			err = find_var_address(pc_iterator->param2, &value);
		}
			
		//if no error occured:
		if (err == 0)
		{
			parse_int(value, &program[c]);
			c += 4;
		}
		else if (err != -1)
			return 1;
			
	}
	
	return 0;
}

void Assembler::parse_int(int param, uint8_t* destination)
{
	
	if (debug)
	{
		printf("\tFUNCTION: parse_int\n");
		printf("\t\tVAR: value = %i\n",param);
	}
	
	// write the int param byte-wise into byte-array destination
	for (int i=0; i<4; i++)
	{
		uint8_t nextbyte = param>>(8*(3-i)) & 0x000000FF;
		//if (debug)
		//	printf("VAR: %i. byte = %i\n", i+1, nextbyte);
		destination[i] = nextbyte;
	}
}

// decode a single command
int Assembler::classify_line(string line_of_code)
{
	/*
		0x00	: STP
		0x01	: JMP
		0x02	: JGZ
		0x03	: JOF
		
		0x04	: ADD
		0x05	: SUB
		0x06	: AND
		0x07	: BOR
		0x08	: SHL
		0x09	: SHR
		
		0x0A	: LDA
		0x0B	: LDB
		0x0C	: LDC
		0x0D	: LD0
		
		0x0E	: STR
		0x0F	: MOV
		
		0x10	: NOP
		0x12	: JEZ
		0x13	: JNO
		
		0x14	: MUL
		0x15	: DIV
		
		0x1A	: RLA
		0x1B	: RLB
		0x1C	: LDM
		0x1D	: LD1
		
		0x24	: SAD
		0x25	: SSU
		
		0x28	: SMU
		0x29	: SDI
		
		0x2C	: STC
		
		0x100	: ERR	(error)
		0x101	: //	(comment)
		0x102	: LAB	(label)
		0x103	: VAR	(declaration of variables)
		0x104	: BEG	(begin-label)
	*/
	
	if (debug)
		printf("FUNCTION: classify_line\n");
	
	char cmd[3];
	string::iterator it = line_of_code.begin();
	for (int i=0; i<3; i++)
	{
		cmd[i]=*it;
		it++;
	}
	
	/* INSTRUCTION-SYMBOLS: */
	
	// case stop:
	if (cmd[0] == 'S' && cmd[1] == 'T' && cmd[2] == 'P')
		return STP;
	
	// case jump:
	if (cmd[0] == 'J')
	{
		if (cmd[1] == 'M' && cmd[2] == 'P')
			return JMP;
		if (cmd[1] == 'G' && cmd[2] == 'Z')
			return JGZ;
		if (cmd[1] == 'O' && cmd[2] == 'F')
			return JOF;
		if (cmd[1] == 'E' && cmd[2] == 'Z')
			return JEZ;
		if (cmd[1] == 'N' && cmd[2] == 'O')
			return JNO;
	}
	
	// case ADD
	if (cmd[0] == 'A' && cmd[1] == 'D' && cmd[2] == 'D')
	{
		return ADD;
	}
	
	// case SUB, shift:
	if (cmd[0] == 'S')
	{
		if (cmd[1] == 'U' && cmd[2] == 'B')
			return SUB;
		if (cmd[1] == 'H')
		{
			if (cmd[2] == 'L')
				return SHL;
			if (cmd[2] == 'R')
				return SHR;
		}
	}
	
	// case AND:
	if (cmd[0] == 'A' && cmd[1] == 'N' && cmd[2] == 'D')
	{
		return AND;
	}
	
	// case BOR:
	if (cmd[0] == 'B' && cmd[1] == 'O' && cmd[2] == 'R')
	{
		return BOR;
	}
	
	// case Load:
	if (cmd[0] == 'L' && cmd[1] == 'D')
	{
		if (cmd[2] == 'A')
			return LDA;
		if (cmd[2] == 'B')
			return LDB;
		if (cmd[2] == 'C')
			return LDC;
		if (cmd[2] == '0')
			return LD0;
		if (cmd[2] == 'M')
			return LDM;
		if (cmd[2] == '1')
			return LD1;
	}
	
	// case store:
	if (cmd[0] == 'S' && cmd[1] == 'T')
	{
		if (cmd[2] == 'R')
			return STR;
		if (cmd[2] == 'C')
			return STC;
	}
	
	// case move:
	if (cmd[0] == 'M' && cmd[1] == 'O' && cmd[2] == 'V')
		return MOV;
		
	// case multiply:
	if (cmd[0] == 'M' && cmd[1] == 'U' && cmd[2] == 'L')
		return MUL;
		
	// case NOP:
	if (cmd[0] == 'N' && cmd[1] == 'O' && cmd[2] == 'P')
		return NOP;

	// extended instructions:
	if (cmd[0] == 'E')
	{
		if (cmd[1] == 'A' && cmd[2] == 'D')
			return EAD;
		if (cmd[1] == 'S' && cmd[2] == 'U')
			return ESU;
		if (cmd[1] == 'M' && cmd[2] == 'U')
			return EMU;
		if (cmd[1] == 'D' && cmd[2] == 'I')
			return EDI;
	}
	
	/* META-SYMBOLS: */
	
	// case comment:
	if (cmd[0] == '/' && cmd[1] == '/')
		return 0x101;
		
	// case label:
	if (cmd[0] == 'L' && cmd[1] == 'A' && cmd[2] == 'B')
		return 0x102;
		
	// case vars:
	if (cmd[0] == 'V' && cmd[1] == 'A' && cmd[2] == 'R')
		return 0x103;
		
	// case begin:
	if (cmd[0] == 'B' && cmd[1] == 'E' && cmd[2] == 'G')
		return 0x104;
		
	// case error:
	return 0x100;
}

int Assembler::read_single_string_parameter(string line_of_code, string* p1)
{
	if (debug)
		printf("FUNCTION: read_single_string_parameter\n");

    int rsize = 10;
    char res[rsize+1];
	std::string::iterator it = line_of_code.begin();
	
	// cut off command and following spaces:
	while(*it != ' ')
		it++;
    it++;
	while(*it == ' ')
		it++;

	// read at most 10 chars or until non-letter is found:
    int pos = 0;
	while(pos <= rsize && (((int)*it > 0x40 && (int)*it < 0x5B) || ((int)*it > 0x60 && (int)*it < 0x7B)))
    {
        res[pos] = *it;
        pos++;
        it++;
    }

    string sres(res,pos);
	if (debug)
		printf("\tstring is: %s\n",sres.c_str());
	*p1 = sres;
	
	return 0;
}

int Assembler::read_double_string_parameter(string line_of_code, string* p1, string* p2)
{
	if (debug)
		printf("FUNCTION: read_double_string_parameter\n");
    int rsize = 10;
    char res[rsize+1];
    int pos;
	std::string::iterator it = line_of_code.begin();
	
	// cut off command and following spaces:
	while(*it != ' ')
		it++;
	
	while(*it == ' ')
		it++;

	// read at most 10 chars or until non-letter is found:
	pos = 0;
	while(pos <= rsize && (((int)*it > 0x40 && (int)*it < 0x5B) || ((int)*it > 0x60 && (int)*it < 0x7B)))
    {
        res[pos] = *it;
        pos++;
        it++;
    }
    string sres1(res,pos);
	
	// move forward until next parameter starts:
	while(*it !=' ')
		it++;
	
	while(*it == ' ')
		it++;
		
	// read at most 10 chars or until non-letter is found:
    pos = 0;
	while(pos <= rsize && (((int)*it > 0x40 && (int)*it < 0x5B) || ((int)*it > 0x60 && (int)*it < 0x7B)))
    {
        res[pos] = *it;
        pos++;
        it++;
    }
	
	string sres2(res,pos);
	if (debug)
		printf("\tstring 1 is: %s\n\tstring 2 is: %s\n",sres1.c_str(), sres2.c_str());
	*p1 = sres1;
	*p2 = sres2;
	
	return 0;
}


int Assembler::read_single_int_parameter(string line_of_code, int* v1)
{
	if (debug)
		printf("FUNCTION: read_single_int_parameter\n");
	int value = 0;
	std::string::iterator it = line_of_code.begin();
	
	// cut off command and following ':' and spaces:
	while(*it != ' ')
		it++;
	
	while(*it == ' ')
		it++;
	
	while ((int)*it >= 0x30 && (int)*it < 0x3A)
	{
		value *= 10;
		value +=(int)*it - 0x30;
		it++;
	}
	if (debug)
		printf("\tvalue is: %i\n",value);
	*v1 = value;
	
	return 0;
}

void Assembler::add_label(string name_label, int address)
{
	if (debug)
		printf("FUNCTION: add_label\n\tlabel : %s, adress = %i\n", name_label.c_str(), address);
	labels.push_back(Pair(name_label, address));
	number_of_labels++;
}

void Assembler::add_var(string name_var, int address)
{
	if (debug)
		printf("FUNCTION: add_var\n\tvar : %s, adress = %i\n", name_var.c_str(), address);
	variables.push_back(Pair(name_var, address));
	number_of_vars++;
}

int Assembler::find_label_address(string name_label, int* address)
{
	if (debug)
		printf("FUNCTION: find_label_address\n");
	// iterate through label-list until name is found, return address
	for (list<Pair>::iterator label_iterator = labels.begin(); label_iterator != labels.end(); label_iterator++)
	{
		if (label_iterator->name.compare(name_label) == 0)
		{
			*address = label_iterator->value;
			if (debug)
				printf("\t label: %s, address: %i\n", name_label.c_str(), *address);
			return 0;
		}			
	}
	// error: label not found
	return 1;
}

int Assembler::find_var_address(string name_var, int* address)
{
	if (debug)
		printf("FUNCTION: find_var_address\n");
	// iterate through variable-list until name is found, return address
	for (list<Pair>::iterator var_iterator = variables.begin(); var_iterator != variables.end(); var_iterator++)
	{
		if (var_iterator->name.compare(name_var) == 0)
		{
			*address = var_iterator->value;
			if (debug)
				printf("\t var: %s, address: %i\n", name_var.c_str(), *address);
			return 0;
		}			
	}
	// error: variable not found
	return 1;
}


int Assembler::assemble(bool debug)
{
	// makes machine-code from read Assemblercode
	// returns 0 if successful, otherwise error-code:
	/*
		1	: error while decoding command
		2	: error while parsing parameter
		3	: label or name not found
	*/
	if (debug)
	{
		printf("FUNCTION: assemble\n");
		printf("VAR: filename_input = %s\n", filename_input);
	}
	
	this->debug = debug;
	
	int err;
	
	err = make_preprogram();
	
	if (debug)
		printf("FINISHED make_preprogram\n\tERR = %i\n\n",err);
	
	if (err == 0)
	{
		err = make_program();
		if (debug)
			printf("FINISHED make_program\n\tERR = %i\n\n",err);
		if (err != 0)
			return err + 1;
	} else
		return err;
	
	return 0;
}

/*
----------------------------------------------------------------------------------------
	OUTPUT 
----------------------------------------------------------------------------------------
*/

// writes machine-code program to file:
void Assembler::write_code()
{
	
	if (debug)
		printf("FUNCTION: write_code\n");
		
	FILE *f = fopen(filename_output, "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		return;
	}
	fprintf(f, "%i\n",length_of_code);
	for (int i=0; i<length_of_code; i++)
	{
		if (program[i] != 0)
			fprintf(f, "%#04x\n",program[i]);
		else
			fprintf(f, "0x00\n",program[i]);
			
	}
		
	fclose(f);
}

/*
----------------------------------------------------------------------------------------
	DEBUG 
----------------------------------------------------------------------------------------
*/

// prints precode and all informations to screen:
void Assembler::print_precode()
{
	printf("\nSTATISTICS:\n\tnumber of commands: %i\n\tnumber of labels: %i\n\tnumber of vars: %i\n",number_of_commands, number_of_labels, number_of_vars);
	printf("\nPRECODE:\n");
	for (list<Precode>::iterator pc_iterator = preprogram.begin(); pc_iterator != preprogram.end(); pc_iterator++)
	{
		uint8_t code = pc_iterator->code;
		int adr = pc_iterator->address;
		
		// case no params:
		if (code == 0x4 || code == 0x5 || code == 0x6 || code == 0x7 || code == 0x8 || code == 0x9 || code == 0xF || code == 0x0)
			printf("%3i %i\n", adr, code);
		
		// case one string param:
		if (code == 0x1 || code == 0x2 || code == 0x3 || code == 0xA || code == 0xB || code == 0xD || code == 0xE)
			printf("%3i %i %s\n", adr, code, pc_iterator->param1.c_str());
		
		// case constant value:
		if (code == 0xC)
			printf("%3i %i %i\n", adr, code, pc_iterator->val);
			
		// case MOV (two string params):
		if (code == 0xE)
			printf("%3i %i %s %s\n", adr, code, pc_iterator->param1.c_str(),  pc_iterator->param2.c_str());
	}
	
	printf("\nVARIABELS:\n");
	for (list<Pair>::iterator var_iterator = variables.begin(); var_iterator != variables.end(); var_iterator++)
		printf("\t%s at %i\n",var_iterator->name.c_str(), var_iterator->value);
	
		printf("\nLABELS:\n");
	for (list<Pair>::iterator label_iterator = labels.begin(); label_iterator != labels.end(); label_iterator++)
		printf("\t%s at %i\n",label_iterator->name.c_str(), label_iterator->value);
	
}
