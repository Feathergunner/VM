#ifndef CommunicationUnit_h
#define CommunicationUnit_h

#include <vector>
#include <stdint.h>

#include "CommunicationChannel.hpp"

class CommunicationUnit
{
private:
	int number_of_extern_devices;
	std::vector<CommunicationChannel> channels;
	
public:
	// constructor
	CommunicationUnit(int num_of_dev);
	// destructor
	
	// get byte from extern
	uint8_t get_extern_value(int devicenumber, bool* succ);
	// store byte to communication-register:
	void provide_value(uint8_t byte, int devicenumber, bool* succ);
};

#endif
