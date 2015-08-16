#ifndef instructions_asm
#define instructions_asm

#include <stdint.h>

//							num of params:

// extended artihmetical operations:
// combined ADD with parameter-load into registers:
const uint8_t EAD = 0x24;	// 2
// comined SUB as above:
const uint8_t ESU = 0x25;	// 2

// comined MUL as above:
const uint8_t EMU = 0x28;	// 2
// comined SUB as above:
const uint8_t EDI = 0x29;	// 2

// not yet implemented:
// store a constant in RAM - (1): adr (2): const value
const uint8_t STC = 0x2C;	// 2

#endif
