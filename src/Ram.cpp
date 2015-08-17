#include "Ram.hpp"

Ram::Ram()
{
	size = 0x1000;
	memory = (uint8_t*)malloc(size);
	memset(memory, 0, size);
}

Ram::Ram(uint32_t size)
{
	this->size = size;
	memory = (uint8_t*)malloc(size);
	memset(memory, 0, size);
}

Ram::~Ram()
{
	free(memory);
}

// check if address is within address-space. otherwise take module
// realizes circular organized memory
uint32_t Ram::check_address(uint32_t address)
{
	if (address < size)
		return address;
	else 
		return (address % size);
}

/*
----------------------------------------------------------------------------------------
	READING 
----------------------------------------------------------------------------------------
*/

// get 8 bit from address
uint8_t Ram::get_byte(uint32_t address)
{
	return memory[check_address(address)];
}

// get 1+BYTESIZE_OF_ADRESSSPACE * 8 bit from address
uint32_t Ram::get_int(uint32_t address)
{
	//if (debug)
	//	printf("\nstart reading 32bit-int from ram.\nadress is %#x\n", address);
	uint32_t res = 0;
	uint8_t nextbyte;
	// read the bytes and shift them correctly:
	for (int i=0; i < BYTESIZE_OF_ADRESSSPACE; i++)
	{
		nextbyte = memory[check_address(address+i)];
		res += nextbyte<<(8*(BYTESIZE_OF_ADRESSSPACE-1-i));
	//	if (debug)
	//		printf("res now is: %i\nnext byte was: %i\n", res, nextbyte);
	}
	return res;
}

/*
----------------------------------------------------------------------------------------
	WRITING 
----------------------------------------------------------------------------------------
*/

// writes 8 bit to address
void Ram::store_byte(uint8_t data, uint32_t address)
{
	memory[check_address(address)] = data;
	memory[address] = data;
}

// writes 32 bit to address
void Ram::store_int(uint32_t data, uint32_t address)
{
	//if (debug)
	//	printf("\nstart writing 32bit-int %#x to ram.\nadress is %#x\n", data, address);
	uint8_t nextbyte;
	for (int i=0; i<BYTESIZE_OF_ADRESSSPACE; i++)
	{
		nextbyte = (data>>(8*(BYTESIZE_OF_ADRESSSPACE-1-i))) & 0xFF;
		memory[check_address(address+i)] = nextbyte;
	//	if (debug)
	//		printf("wrote %#x at ram[%#x = %i]\n", nextbyte, address+i, address+i);
	}
}

void Ram::move(uint32_t source, uint32_t destination)
{
	// copies data from source to destination
	memory[check_address(destination)] = memory[check_address(source)];
}

/*
----------------------------------------------------------------------------------------
	DEBUG 
----------------------------------------------------------------------------------------
*/

void Ram::print_ram(const char* filename)
{
	// writes memory to file
	FILE* ramfile;
	ramfile = fopen(filename, "w");
	for (int i=0; i<size; i++)
		fprintf(ramfile, "%#X\n", memory[i]);
	
	fclose(ramfile);
}
