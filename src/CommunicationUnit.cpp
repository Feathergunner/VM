#include "CommunicationUnit.hpp"

CommunicationUnit::CommunicationUnit(int num_of_dev)
{
	#ifdef DEBUG
		printf("CONSTRUCTOR: %s\n", __PRETTY_FUNCTION__);
	#endif

	number_of_extern_devices = num_of_dev;
	channels.reserve(num_of_dev);
	flag_success = false;
}

int CommunicationUnit::get_number_of_devices()
{
	#ifdef DEBUG
		printf("FUNCTION: %s\n", __PRETTY_FUNCTION__);
	#endif

	return number_of_extern_devices;
}

uint32_t CommunicationUnit::get_extern_value(int devicenumber)
{
	#ifdef DEBUG
		printf("FUNCTION: %s\n", __PRETTY_FUNCTION__);
	#endif

	uint32_t res = channels[devicenumber].read_value_from_channel(&flag_success);
	return res;
}

void CommunicationUnit::provide_value(uint32_t byte, int devicenumber)
{
	#ifdef DEBUG
		printf("FUNCTION: %s\n", __PRETTY_FUNCTION__);
	#endif

	channels[devicenumber].write_value_to_channel(byte, &flag_success);
	return;
}

bool CommunicationUnit::check_communication_success()
{
	#ifdef DEBUG
		printf("FUNCTION: %s\n", __PRETTY_FUNCTION__);
	#endif

	return flag_success;
	//bool lock_status = channels[devicenumber].get_lock_status();
	//return lock_status;
}