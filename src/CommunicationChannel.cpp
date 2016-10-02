#include "CommunicationChannel.hpp"

/*
CommunicationChannel::CommunicationChannel(CommunicationChannel* ext_chan)
{
	#ifdef DEBUG
		printf("CONSTRUCTOR: %s\n", __PRETTY_FUNCTION__);
	#endif

	reg = 0;
	flag_lock = false;
	this->extern_channel = ext_chan;
}
*/

CommunicationChannel::CommunicationChannel()
{

	#ifdef DEBUG
		printf("CONSTRUCTOR: %s\n", __PRETTY_FUNCTION__);
	#endif
	
	reg = 0;
	flag_lock = false;
	this->extern_channel = this;	
}

void CommunicationChannel::set_extern_device(CommunicationChannel* ext_chan)
{
	this->extern_channel = ext_chan;
}

void CommunicationChannel::write_value_to_channel(uint32_t data, bool* succ)
{
	if (!flag_lock)
	{
		reg = data;
		flag_lock = true;
		*succ = true;
	}
	else
	{
		*succ = false;
	}
	return;
}

uint32_t CommunicationChannel::read_value_from_channel(bool* succ)
{
	return extern_channel->get_value(succ);
}

uint32_t CommunicationChannel::get_value(bool* succ)
{
	if (flag_lock)
	{
		flag_lock = false;
		*succ = true;
		return reg;
	}
	else
	{
		*succ = false;
		return 0;
	}
}

/*
bool CommunicationChannel::get_lock_status()
{
	return flag_lock;
}
*/