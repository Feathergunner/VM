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
	
	void write_byte_to_channel(uint8_t data, bool* succ);
	uint8_t read_byte_from_channel(bool* succ);
	uint8_t get_byte(bool* succ);
};

#endif
