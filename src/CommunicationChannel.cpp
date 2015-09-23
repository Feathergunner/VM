#include "CommunicationChannel.hpp"

void CommunicationChannel::write_byte_to_channel(uint8_t data, bool* succ)
{
	if (!flag_lock)
	{
		reg = byte;
		flag_lock = true;
		&succ = true;
	}
	else
	{
		&succ = false;
	}
	return;
}

uint8_t CommunicationChannel::read_byte_from_channel(bool* succ)
{
	return extern_channel.get_byte(succ);
}

uint8_t CommunicationChannel::get_byte(bool* succ)
{
	if (flag_lock)
	{
		flag_lock = false;
		&succ = true
		return reg;
	}
	else
	{
		&succ = false;
		return 0;
	}
}
