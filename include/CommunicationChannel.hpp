#ifndef Communicationchannel_h
#define Communicationchannel_h

#include <stdint.h>

class CommunicationChannel
{
private:
	uint8_t reg;
	bool flag_lock;
	
	CommunicationChannel extern_channel;
	
public:
	// constructor:
	CommunicationChannel();
	CommunicationChannel(CommunicationChannel ext_chan);
	
	// init:
	void set_extern_device(CommunicationChannel ext_chan);
	
	void write_value_to_channel(uint32_t value, bool* succ);
	uint32_t read_value_from_channel(bool* succ);
	uint32_t get_value(bool* succ);
};

#endif
