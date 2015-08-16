#ifndef Ram_h
#define Ram_h

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class Ram
{
private:
	uint8_t* memory;
	uint32_t size;

	uint32_t check_address(uint32_t address);
public:
	// constructor:
	Ram(uint32_t size);
	Ram();
	// destructor:
	~Ram();
	
	// get data:
	uint8_t get_byte(uint32_t address);
	uint32_t get_int(uint32_t address);
	
	// store data:
	void store_byte(uint8_t data, uint32_t address);
	void store_int(uint32_t data, uint32_t address);
	
	// move data:
	void move(uint32_t source, uint32_t destination);
	
	// debug:
	void print_ram(const char* filename);
};

#endif