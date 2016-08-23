#include "CommunicationUnit.hpp"

CommunicationUnit::CommunicationUnit(int num_of_dev)
{
	number_of_extern_devices = num_of_dev;
	channels.reserve(num_of_dev);
}
uint8_t CommunicationUnit::get_extern_value(int devicenumber, bool* succ)
{
	byte res = channels[devicenumber].read_byte(succ);
	return res;
}

void CommunicationUnit::provide_value(uint8_t byte, int devicenumber, bool* succ)
{
	channels[devicenumber].write_byte(byte, succ);
	return;
}
