#ifnded Communicationchannel_h
#define Communicationchannel_h

#include <stdint.h>

class CommunicationChannel
{
private:
	uint8_t reg;
	bool flag_lock;
	
	CommunicationChannel extern_device;
	
public:
	// constructor:
	CommunicationChannel();
	CommunicationChannel(CommunicationChannel ext_dev);
	
	// init:
	void set_extern_device(CommunicationChannel ext_dev);
	
	void write_byte(uint8_t data);
	uint8_t read_byte();
};

#endif