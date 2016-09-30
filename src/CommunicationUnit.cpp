#include "CommunicationUnit.hpp"

CommunicationUnit::CommunicationUnit(int num_of_dev)
{
	number_of_extern_devices = num_of_dev;
	channels.reserve(num_of_dev);
}
uint32_t CommunicationUnit::get_extern_value(int devicenumber, bool* succ)
{
	uint32_t res = channels[devicenumber].read_value_from_channel(succ);
	return res;
}

void CommunicationUnit::provide_value(uint32_t byte, int devicenumber, bool* succ)
{
	channels[devicenumber].write_value_to_channel(byte, succ);
	return;
}

bool CommunicationUnit::check_lock_status(int devicenumber)
{
	bool lock_status = channels[devicenumber].get_lock_status();
	return lock_status;
}