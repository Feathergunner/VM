#include "CommunicationUnit.hpp"

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
