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
	
	if (debug > 0)
		printf("\n[FUNCTION: make_preprogram]\n");
	
	// set filestream:
	inputfile.open(filename_input, ifstream::in);

	int maxchars = 60;
	char line_of_code[maxchars];
	
	// add initial jump to begin
	string label_begin("BEGIN");
	preprogram.push_back(Precode(JMP, 0, label_begin));
	number_of_commands++;
	// length_of_code += 5;
	
	// read from file:
	inputfile.getline(line_of_code, maxchars);
	int line_number = 1;
	offset = 0;
	
	while(inputfile.good())
	{
		if (debug > 1)
			printf("\n\tnext line_of_code = \n\t\t%i %s\n", line_number, line_of_code);
		
		int type = classify_line(line_of_code);
		if (debug > 1)
			printf("\ttype = \n\t\t %i\n",type);
		
		// check if error
		if (type == 0x100)
			return 1; // error
		
		// label
		if (type == 0x102)
		{
			// add label to next position in memory
			// get name:
			string name_label;
			std::string::iterator it = ((string)line_of_code).begin();
			read_string_parameter(&it, &name_label);
			//read_single_string_parameter(line_of_code, &name_label);
			add_label(name_label, offset);
			//if (debug)
			//	printf("PRECODE: added label %s at offset %i\n",name_label, offset);
		}
		
		// var
		if (type == 0x103)
		{
			// declaration of variable
			// get name:
			string var_name;
			std::string::iterator it = ((string)line_of_code).begin();
			read_string_parameter(&it, &var_name);
			// add var:
			int var_adress = (1+BYTESIZE_OF_ADRESSSPACE)  + (BYTESIZE_OF_ADRESSSPACE*number_of_vars);
			add_var(var_name, var_adress);
			
			//if (debug)
			//	printf("PRECODE: added var %s at adress %i\n", var_name, var_adress);
			
		}
			
		// begin-label
		if (type == 0x104)
		{
			add_label(label_begin, offset);
			if (debug > 1)
				printf("\tPRECODE: added BEGIN-Label at offset %i\n", offset);	
		}
			
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
				std::string::iterator it = ((string)line_of_code).begin();
				err = read_string_parameter(&it, &p1);
				// if no error occured, set precode:
				if (err == 0)
				{
					preprogram.push_back(Precode(type, offset, p1));
					if (debug > 1)
						printf("\tPRECODE: added \n\t\tcommand %i\n\t\tat offset %i\n", type, offset);	
				}
				else
					return 2;
					
				offset += 1+BYTESIZE_OF_ADRESSSPACE;
			}
			
			if (type == LDC)
			{
				// case load_constant: read integer:
				// read one integer parameter:
				std::string::iterator it = ((string)line_of_code).begin();
				err = read_int_parameter(&it, &val1);
				// if no error occured, set precode:
				if (err == 0)
				{
					preprogram.push_back(Precode(type, offset, val1));
					if (debug > 1)
						printf("\tPRECODE: added \n\t\tcommand %i\n\t\tat offset %i\n", type, offset);	
				}
				else
					return 2;
					
				offset += 1+BYTESIZE_OF_ADRESSSPACE;
			}
			
			if (type == MOV) 
			{
				// case move: to variable-names
				// read two parameters:
				std::string::iterator it = ((string)line_of_code).begin();
				err = read_string_parameter(&it, &p1);
				err = read_string_parameter(&it, &p2);
				// if no error occured, set precode:
				if (err == 0)
				{
					preprogram.push_back(Precode(type, offset, p1, p2));
					if (debug > 1)
						printf("\tPRECODE: added \n\t\tcommand %i\n\t\tat offset %i\n", type, offset);	
				}
				else
					return 2;
			
				offset += 1 + 2*BYTESIZE_OF_ADRESSSPACE;
			}	
			
			if (type == ADD || type == SUB || type == AND || type == BOR || type == MUL || type == DIV || type == SHL || type == SHR || type == LD0 || type == LDM || type == LD1 || type == RLA || type == RLB || type == NOP || type == STP)
			{
				// no parameters:
				preprogram.push_back(Precode(type, offset));
				if (debug > 1)
						printf("\tPRECODE: added \n\t\tcommand %i\n\t\tat offset %i\n", type, offset);	
				
				offset += 1;
			}
			
			// extended instructions:
			if (type == EAD || type == ESU || type == EMU || type == EDI)
			{
				// get operands:
				std::string::iterator it = ((string)line_of_code).begin();
				err = read_string_parameter(&it, &p1);
				err = read_string_parameter(&it, &p2);
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
			
			if (type == STC)
			{
				std::string::iterator it = ((string)line_of_code).begin();
				err = read_int_parameter(&it, &val1);
				err = read_string_parameter(&it, &p1);
				//err = read_double_int_parameter(line_of_code, &val1, &val2);
				if (err == 0)
					make_STC(val1, p1);
			}
			
		}
		
		inputfile.getline(line_of_code, maxchars);
		line_number++;
	}
	
	inputfile.close();
	
	// length of header (initial jump + all vars)
	// i.e. the basis for all precomputed adress-offsets:
	length_of_header = (1 + BYTESIZE_OF_ADRESSSPACE) + (BYTESIZE_OF_ADRESSSPACE*number_of_vars);

	length_of_code = length_of_header + offset;
	
	update_jump_labels();
	
	return 0;
}

// update jump-labels after first iteration, when all adresses are known
void Assembler::update_jump_labels()
{
	if (debug > 1)
		printf("[FUNCTION: update_jump_labels]");
	for (list<Pair>::iterator it = labels.begin(); it != labels.end(); it++)
		it->value += length_of_header;
}

/*
---------------------------------------------------------------------------------------
	UNROLL EXTENDED ASSEMBLER INSTRUCTIONS
---------------------------------------------------------------------------------------
*/

void Assembler::make_eADD(string param1, string param2)
{
	preprogram.push_back(Precode(LDA, offset, param1));
	if (debug > 1)
		printf("\tPRECODE: added \n\t\tcommand %i\n\t\tat offset %i\n", LDA, offset);	
	offset += 1+BYTESIZE_OF_ADRESSSPACE;
	preprogram.push_back(Precode(LDB, offset, param2));
	if (debug > 1)
		printf("\tPRECODE: added \n\t\tcommand %i\n\t\tat offset %i\n", LDB, offset);	
	offset += 1+BYTESIZE_OF_ADRESSSPACE;
	preprogram.push_back(Precode(ADD, offset));
	if (debug > 1)
		printf("\tPRECODE: added \n\t\tcommand %i\n\t\tat offset %i\n", ADD, offset);	
	offset += 1;
}

void Assembler::make_eSUB(string param1, string param2)
{
	preprogram.push_back(Precode(LDA, offset, param1));
	if (debug > 1)
		printf("\tPRECODE: added \n\t\tcommand %i\n\t\tat offset %i\n", LDA, offset);	
	offset += 1+BYTESIZE_OF_ADRESSSPACE;
	preprogram.push_back(Precode(LDB, offset, param2));
	if (debug > 1)
		printf("\tPRECODE: added \n\t\tcommand %i\n\t\tat offset %i\n", LDB, offset);	
	offset += 1+BYTESIZE_OF_ADRESSSPACE;
	preprogram.push_back(Precode(SUB, offset));
	if (debug > 1)
		printf("\tPRECODE: added \n\t\tcommand %i\n\t\tat offset %i\n", SUB, offset);	
	offset += 1;
}

void Assembler::make_eMUL(string param1, string param2)
{
	preprogram.push_back(Precode(LDA, offset, param1));
	if (debug > 1)
		printf("\tPRECODE: added \n\t\tcommand %i\n\t\tat offset %i\n", LDA, offset);	
	offset += 1+BYTESIZE_OF_ADRESSSPACE;
	preprogram.push_back(Precode(LDB, offset, param2));
	if (debug > 1)
		printf("\tPRECODE: added \n\t\tcommand %i\n\t\tat offset %i\n", LDB, offset);	
	offset += 1+BYTESIZE_OF_ADRESSSPACE;
	preprogram.push_back(Precode(MUL, offset));
	if (debug > 1)
		printf("\tPRECODE: added \n\t\tcommand %i\n\t\tat offset %i\n", MUL, offset);	
	offset += 1;
}

void Assembler::make_eDIV(string param1, string param2)
{
	preprogram.push_back(Precode(LDA, offset, param1));
	if (debug > 1)
		printf("\tPRECODE: added \n\t\tcommand %i\n\t\tat offset %i\n", LDA, offset);	
	offset += 1+BYTESIZE_OF_ADRESSSPACE;
	preprogram.push_back(Precode(LDB, offset, param2));
	if (debug > 1)
		printf("\tPRECODE: added \n\t\tcommand %i\n\t\tat offset %i\n", LDB, offset);	
	offset += 1+BYTESIZE_OF_ADRESSSPACE;
	preprogram.push_back(Precode(DIV, offset));
	if (debug > 1)
		printf("\tPRECODE: added \n\t\tcommand %i\n\t\tat offset %i\n", DIV, offset);	
	offset += 1;
}

void Assembler::make_STC(int value, string var_name)
{
	preprogram.push_back(Precode(LDC, offset, value));
	if (debug > 1)
		printf("PRECODE: added \n\tcommand %i\n\tat offset %i\n", LDC, offset);
	offset += 1+BYTESIZE_OF_ADRESSSPACE;
	preprogram.push_back(Precode(LD0, offset));
	if (debug > 1)
		printf("PRECODE: added \n\tcommand %i\n\tat offset %i\n", LD0, offset);
	offset += 1;
	preprogram.push_back(Precode(ADD, offset));
	if (debug > 1)
		printf("PRECODE: added \n\tcommand %i\n\tat offset %i\n", ADD, offset);
	offset += 1;
	preprogram.push_back(Precode(STR, offset, var_name));
	if (debug > 1)
		printf("PRECODE: added \n\tcommand %i\n\tat offset %i\n", STR, offset);
	offset += 1+BYTESIZE_OF_ADRESSSPACE;
}

// second iteration:
//	- insert correct addresses for variables and jump-addresses
int Assembler::make_program()
{	
	if (debug > 0)
		printf("\n[FUNCTION: make_program]\n");
	if (debug > 1)
		printf("\tinitialize %i bytes for program\n",length_of_code);
	
	// create byte-array for program:
	program = (uint8_t*)malloc(length_of_code);
	memset(program, 0, length_of_code);
	
	// initialize byte-counter:
	int c = 0;
	
	// iterate through preprogram:
	for (list<Precode>::iterator pc_iterator = preprogram.begin(); pc_iterator != preprogram.end(); pc_iterator++)
	{
		if (debug > 1)
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
		if (debug > 1)
			printf("VAR: code at byte %i: %i\n",pc_iterator->address,code);
		
		//check if current command is command with parameters and add them to program:
		int value, err = -1;
		
		// case jump-label:
		if (code == JMP || code == JGZ || code == JOF || code == JEZ || code == JNO)
		{
			if (debug > 1)
				printf("CASE: jump - get address\n");
			err = find_label_address(pc_iterator->param1, &value);
		}
		
		// case var-name:
		if (code == LDA || code == LDB || code == STR || code == MOV)
		{
			if (debug > 1)
				printf("CASE: variable - get address\n");
			err = find_var_address(pc_iterator->param1, &value);
		}
		
		// case constant value:
		if (code == LDC)
		{
			if (debug > 1)
				printf("Case: constant value\n");
			value = pc_iterator->val;
			// set err to 0 to flag for parsing:
			err = 0;
		}
			
		// if no error occured:
		if (err == 0)
		{
			parse_int(value, &program[c]);
			c += BYTESIZE_OF_ADRESSSPACE;
		}
		else if (err != -1)
			return 1;
			
		err = -1;
			
		// check if current command has second parameter, and add it to program:
		// case MOV:
		if (code == MOV)
		{
			if (debug > 1)
				printf("Case: move - get second address\n");
			err = find_var_address(pc_iterator->param2, &value);
		}
			
		//if no error occured:
		if (err == 0)
		{
			parse_int(value, &program[c]);
			c += BYTESIZE_OF_ADRESSSPACE;
		}
		else if (err != -1)
			return 1;
			
	}
	
	return 0;
}

void Assembler::parse_int(int param, uint8_t* destination)
{
	
	if (debug > 1)
	{
		printf("[FUNCTION: parse_int]\n");
		printf("\t\tVAR: value = %i\n",param);
	}
	
	// write the int param byte-wise into byte-array destination
	for (int i=0; i<BYTESIZE_OF_ADRESSSPACE; i++)
	{
		uint8_t nextbyte = param>>(8*(BYTESIZE_OF_ADRESSSPACE-1-i)) & 0x000000FF;
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
	
	if (debug > 1)
		printf("[FUNCTION: classify_line]\n");
	
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
	
	// case ADD, AND
	if (cmd[0] == 'A')
	{
		if (cmd[1] == 'D' && cmd[2] == 'D')
			return ADD;
		if (cmd[1] == 'N' && cmd[2] == 'D')
			return AND;
	}
		
	// case BOR:
	if (cmd[0] == 'B' && cmd[1] == 'O' && cmd[2] == 'R')
	{
		return BOR;
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
		
	// case divide:
	if (cmd[0] == 'D' && cmd[1] == 'I' && cmd[2] == 'V')
		return DIV;
		
	// case reload
	if (cmd[0] == 'R' && cmd[1] == 'L')
	{
		if (cmd[2] == 'A')
			return RLA;
		if (cmd[2] == 'B')
			return RLB;
	}
		
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
	
	// ext: store const:
	if (cmd[0] == 'S' && cmd[1] == 'T' && cmd[2] == 'C')
		return STC;
	
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

int Assembler::read_int_parameter(std::string::iterator* it, int* i_val)
{
	// cut off command and following spaces:
	while(*(*it) != ' ')
		(*it)++;
    //(*it)++;
	while(*(*it) == ' ')
		(*it)++;
		
	if (debug > 1)
		printf("[FUNCTION: read_int_parameter]\n");
	int value = 0;
	while ((int)*(*it) >= 0x30 && (int)*(*it) < 0x3A)
	{
		value *= 10;
		value +=(int)*(*it) - 0x30;
		(*it)++;
	}
	if (debug > 1)
		printf("\tvalue is: %i\n",value);
	*i_val = value;
	
	return 0;
}

int Assembler::read_string_parameter(std::string::iterator* it, string* s_val)
{
	// cut off command and following spaces:
	while(*(*it) != ' ')
		(*it)++;
    //(*it)++;
	while(*(*it) == ' ')
		(*it)++;
		
	if (debug > 1)
		printf("[FUNCTION: read_string_parameter]\n");
	// read at most 10 chars or until non-letter is found:
	int rsize = 10;
    char res[rsize+1];
    int pos = 0;
	while(pos <= rsize && (((int)*(*it) > 0x40 && (int)*(*it) < 0x5B) || ((int)*(*it) > 0x60 && (int)*(*it) < 0x7B)))
    {
        res[pos] = *(*it);
        pos++;
        (*it)++;
    }

    string sres(res,pos);
	if (debug > 1)
		printf("\tstring is: %s\n",sres.c_str());
	*s_val = sres;
	
	return 0;
}

void Assembler::add_label(string name_label, int address)
{
	if (debug > 1)
		printf("[FUNCTION: add_label]\n\tlabel : %s, adress = %i\n", name_label.c_str(), address);
	labels.push_back(Pair(name_label, address));
	number_of_labels++;
}

void Assembler::add_var(string name_var, int address)
{
	if (debug > 1)
		printf("[FUNCTION: add_var]\n\tvar : %s, adress = %i\n", name_var.c_str(), address);
	variables.push_back(Pair(name_var, address));
	number_of_vars++;
}

int Assembler::find_label_address(string name_label, int* address)
{
	if (debug > 1)
		printf("[FUNCTION: find_label_address]\n");
	// iterate through label-list until name is found, return address
	for (list<Pair>::iterator label_iterator = labels.begin(); label_iterator != labels.end(); label_iterator++)
	{
		if (label_iterator->name.compare(name_label) == 0)
		{
			*address = label_iterator->value;
			if (debug > 1 )
				printf("\t label: %s, address: %i\n", name_label.c_str(), *address);
			return 0;
		}			
	}
	// error: label not found
	return 1;
}

int Assembler::find_var_address(string name_var, int* address)
{
	if (debug > 1)
		printf("[FUNCTION: find_var_address]\n");
	// iterate through variable-list until name is found, return address
	for (list<Pair>::iterator var_iterator = variables.begin(); var_iterator != variables.end(); var_iterator++)
	{
		if (var_iterator->name.compare(name_var) == 0)
		{
			*address = var_iterator->value;
			if (debug > 1)
				printf("\t var: %s, address: %i\n", name_var.c_str(), *address);
			return 0;
		}			
	}
	// error: variable not found
	return 1;
}


int Assembler::assemble(int debug)
{
	// makes machine-code from read Assemblercode
	// returns 0 if successful, otherwise error-code:
	/*
		1	: error while decoding command
		2	: error while parsing parameter
		3	: label or name not found
	*/
	if (debug > 1)
	{
		printf("[FUNCTION: assemble]\n");
		printf("VAR: filename_input = %s\n", filename_input);
	}
	
	this->debug = debug;
	
	int err;
	
	err = make_preprogram();
	
	if (debug > 1)
		printf("FINISHED make_preprogram\n\tERR = %i\n\n",err);
	
	if (err == 0)
	{
		err = make_program();
		if (debug > 1)
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
	
	if (debug > 1)
		printf("[FUNCTION: write_code]\n");
		
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
		if (code == ADD || code == SUB || code == AND || code == BOR || code == SHL || code == SHR ||
			code == LD0 || code == STP ||
			code == NOP || code == MUL || code == DIV || code == RLA || code == RLB || code == LDM || code == LD1)
			printf("%3i %i\n", adr, code);
		
		// case one string param:
		if (code == JMP || code == JGZ || code == JOF || code == JEZ || code == JNO ||
			code == LDA || code == LDB || code == STR)
			printf("%3i %i %s\n", adr, code, pc_iterator->param1.c_str());
		
		// case constant value:
		if (code == LDC)
			printf("%3i %i %i\n", adr, code, pc_iterator->val);
			
		// case MOV (two string params):
		if (code == MOV)
			printf("%3i %i %s %s\n", adr, code, pc_iterator->param1.c_str(),  pc_iterator->param2.c_str());
	}
	
	printf("\nVARIABELS:\n");
	for (list<Pair>::iterator var_iterator = variables.begin(); var_iterator != variables.end(); var_iterator++)
		printf("\t%s at %i\n",var_iterator->name.c_str(), var_iterator->value);
	
		printf("\nLABELS:\n");
	for (list<Pair>::iterator label_iterator = labels.begin(); label_iterator != labels.end(); label_iterator++)
		printf("\t%s at %i\n",label_iterator->name.c_str(), label_iterator->value);
	
}
