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
	bool flag_success;
	
public:
	// constructor
	CommunicationUnit(int num_of_dev);
	// destructor
	~CommunicationUnit(){};

	void set_extern_CommunicationChannel(int id, CommunicationChannel* ext_dev);
	CommunicationChannel* get_communication_channel_link(int id);

	int get_number_of_devices();
	
	// get value from extern:
	uint32_t get_extern_value(int devicenumber);
	// store value to communication-register:
	void provide_value(uint32_t value, int devicenumber);
	// check if latest communication attempt was successful:
	bool check_communication_success();
};

#endif
