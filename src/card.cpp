#include "card.h"
#include <cstring>

Card::Card(uint8_t uid[8], uint8_t type) : m_type(type)
{
	std::memcpy(&m_uid, uid, 8);
}

Card::~Card()
{}

bool    Card::operator==(Card &other)
{
	return m_uid == other.m_uid;
}
