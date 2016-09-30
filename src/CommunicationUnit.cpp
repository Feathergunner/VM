#include "CommunicationUnit.hpp"

CommunicationUnit::CommunicationUnit(int num_of_dev)
{
	number_of_extern_devices = num_of_dev;
	channels.reserve(num_of_dev);
	flag_success = false;
}

int CommunicationUnit::get_number_of_devices()
{
	return number_of_extern_devices;
}

uint32_t CommunicationUnit::get_extern_value(int devicenumber)
{
	uint32_t res = channels[devicenumber].read_value_from_channel(&flag_success);
	return res;
}

void CommunicationUnit::provide_value(uint32_t byte, int devicenumber)
{
	channels[devicenumber].write_value_to_channel(byte, &flag_success);
	return;
}

bool CommunicationUnit::check_communication_success()
{
	return flag_success;
	//bool lock_status = channels[devicenumber].get_lock_status();
	//return lock_status;
}