#ifndef instructions_vm
#define instructions_vm

#include <stdint.h>

//							num of params:
/* SUPER-RISC INSTRUCTION SET */
// stop program:
const uint8_t STP = 0x00;	// 0

// unconditional jump:
const uint8_t JMP = 0x01;	// 1
// jump if register C > 0:
const uint8_t JGZ = 0x02;	// 1
// jump if last operation caused overflow:
const uint8_t JOF = 0x03;	// 1
	
// C = A + B:
const uint8_t ADD = 0x04;	// 0
// C = A - B:
const uint8_t SUB = 0x05;	// 0
// C = A & B:
const uint8_t AND = 0x06;	// 0
// C = A | B:
const uint8_t BOR = 0x07;	// 0
// C = A*2:
const uint8_t SHL = 0x08;	// 0
// C = A div 2:
const uint8_t SHR = 0x09;	// 0

// load from ram into A:
const uint8_t LDA = 0x0A;	// 1
// load from ram into B:
const uint8_t LDB = 0x0B;	// 1
// load constant value into A:
const uint8_t LDC = 0x0C;	// 1
// load 0 into B:
const uint8_t LD0 = 0x0D;	// 0
 
// store from C into ram:
const uint8_t STR = 0x0E;	// 1

// copy a value within ram:
const uint8_t MOV = 0x0F;	// 2

/* RISC ADDITIONAL INSTRUCTIONS */
// no operation:
const uint8_t NOP = 0x10;	// 0

// jump if equal to zero:
const uint8_t JEZ = 0x12;	// 1
// jump of no overflow:
const uint8_t JNO = 0x13;	// 1

// C = A * B
const uint8_t MUL = 0x14;	// 0
// C = A div B
const uint8_t DIV = 0x15;	// 0

// move C -> A
const uint8_t RLA = 0x1A;	// 0
// move C -> B
const uint8_t RLB = 0x1B;	// 0

// load 0xFFFFFFFF in B:
const uint8_t LDM = 0x1C;	// 0
// load 1 in B:
const uint8_t LD1 = 0x1D;	// 0

// send byte to extern device:
const uint8_t SND = 0x1E;	// 1
// get byte from extern device:
const uint8_t RCV = 0x1F;	// 1

#endif
