#ifndef constants
#define constants

#include <stdint.h>
#include <string>
#include <vector>

// size of words and adresses in byte:
const uint8_t WORDSIZE = 2;

// upper bound to number of instructions (ASM):
const uint8_t NUMBER_OF_INSTRUCTIONS = 0x2F;

// upper bound to number of instructions (VM):
const uint8_t NUMBER_OF_INSTRUCTIONS_VM = 0x1F;

// disassemble
// "NU" stands for "not used" and is mapped to NOP by the vm
// every other value greater than NUMBER_OF_INSTRUCTIONS is also NU
const std::vector<std::string> ASM_SYMBOLS = {
"STP",		// 0x00
"JMP",		// 0x01
"JGZ",		// 0x02
"JOF",		// 0x03
"ADD",		// 0x04
"SUB",		// 0x05
"AND",		// 0x06
"BOR",		// 0x07
"SHL",		// 0x08
"SHR",		// 0x09
"LDA",		// 0x0A
"LDB",		// 0x0B
"LDC",		// 0x0C
"LD0",		// 0x0D
"STR",		// 0x0E
"MOV",		// 0x0F

"NOP",		// 0x10
	"NU",	// 0x11
"JEZ",		// 0x12
"JNO",		// 0x13
"MUL",		// 0x14
"DIV",		// 0x15
	"NU",	// 0x16
	"NU",	// 0x17
	"NU",	// 0x18
	"NU",	// 0x19
"RLA",		// 0x1A
"RLB",		// 0x1B
"LDM",		// 0x1C
"LD1",		// 0x1D
"SND",		// 0x1E
"RCV",		// 0x1F

	"NU",	// 0x20
	"NU",	// 0x21
	"NU",	// 0x22
	"NU",	// 0x23
"EAD",		// 0x24
"ESU",		// 0x25
	"NU",	// 0x26
	"NU",	// 0x27
"EMU",		// 0x28
"EDI",		// 0x29
	"NU",	// 0x2A
	"NU",	// 0x2B
"STC",		// 0x2C
	"NU",	// 0x2D
	"NU",	// 0x2E
	"NU"	// 0x2F
};

const std::vector<int> INSTRUCTION_PARAMCOUNT = {
0,		// 0x00
1,		// 0x01
1,		// 0x02
1,		// 0x03
0,		// 0x04
0,		// 0x05
0,		// 0x06
0,		// 0x07
0,		// 0x08
0,		// 0x09
1,		// 0x0A
1,		// 0x0B
1,		// 0x0C
0,		// 0x0D
1,		// 0x0E
2,		// 0x0F

0,		// 0x10
	-1,	// 0x11
1,		// 0x12
1,		// 0x13
1,		// 0x14
1,		// 0x15
	-1,	// 0x16
	-1,	// 0x17
	-1,	// 0x18
	-1,	// 0x19
0,		// 0x1A
0,		// 0x1B
0,		// 0x1C
0,		// 0x1D
1,		// 0x1E
1,		// 0x1F

	-1,	// 0x20
	-1,	// 0x21
	-1,	// 0x22
	-1,	// 0x23
2,		// 0x24
2,		// 0x25
	-1,	// 0x26
	-1,	// 0x27
2,		// 0x28
2,		// 0x29
	-1,	// 0x2A
	-1,	// 0x2B
2,		// 0x2C
	-1,	// 0x2D
	-1,	// 0x2E
	-1	// 0x2F
};
#endif
