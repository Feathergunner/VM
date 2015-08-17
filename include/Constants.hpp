#ifndef constants
#define constants

#include <stdint.h>
#include <string.h>
#include <vector>

// size of adresses in byte:
const uint8_t BYTESIZE_OF_ADRESSSPACE = 2;

// upper bound to number of instructions:
const uint8_t NUMBER_OF_INSTRUCTIONS = 64;

// disassebmle
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
	"NU",	// 0x1E
	"NU",	// 0x1F

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
	"NU",	// 0x2F
};
#endif
