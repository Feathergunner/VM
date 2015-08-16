#ifndef instructions
#define instructions

#include <stdint.h>

//							num of params:
/* SUPER-RISC INSTRUCTION SET */
const uint8_t STP = 0x00;	// 0

const uint8_t JMP = 0x01;	// 1
const uint8_t JGZ = 0x02;	// 1
const uint8_t JOF = 0x03;	// 1
	 
const uint8_t ADD = 0x04;	// 0
const uint8_t SUB = 0x05;	// 0
const uint8_t AND = 0x06;	// 0
const uint8_t BOR = 0x07;	// 0
const uint8_t SHL = 0x08;	// 0
const uint8_t SHR = 0x09;	// 0
	 
const uint8_t LDA = 0x0A;	// 1
const uint8_t LDB = 0x0B;	// 1
const uint8_t LDC = 0x0C;	// 1
const uint8_t LD0 = 0x0D;	// 0
 
const uint8_t STR = 0x0E;	// 1

const uint8_t MOV = 0x0F;	// 2

/* RISC ADDITIONAL INSTRUCTIONS */ // not yet implemented
const uint8_t NOP = 0x10;	// 0

// jump if equal to zero:
const uint8_t JEZ = 0x12;	// 1
// jump of no overflow:
const uint8_t JNO = 0x13;	// 1

// multiply
const uint8_t MUL = 0x14;	// 0
// integer-divide
const uint8_t DIV = 0x15;	// 0

// move C -> A
const uint8_t RLA = 0x1A;	// 0
// move C -> B
const uint8_t RLB = 0x1B;	// 0

// load 0xFFFFFFFF in B:
const uint8_t LDM = 0x1C;	// 0
// load 1 in B:
const uint8_t LD1 = 0x1D;	// 0

#endif
