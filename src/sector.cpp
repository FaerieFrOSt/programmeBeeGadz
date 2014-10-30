#include "sector.h"
#include <cstring>

Sector::Sector(bool isTrailer, size_t nbBlock, bool keyB) :
	m_data(nbBlock), m_state(Sector::DIRTY), m_isTrailer(isTrailer), m_keyB(keyB)
{
	m_key.fill(0xff);
	m_data.shrink_to_fit();
}

Sector::Sector(const Sector &other) : m_data(other.m_data), m_state(other.m_state),
	m_isTrailer(other.m_isTrailer), m_keyB(other.m_keyB)
{
	for (auto &i : m_key)
		i = other.m_key[&i - &m_key[0]];
	m_data.shrink_to_fit();
}

Sector	&Sector::operator=(const Sector &other)
{
	m_isTrailer = other.m_isTrailer;
	m_state = other.m_state;
	m_data = other.m_data;
	m_data.shrink_to_fit();
	for (auto &i : m_key)
		i = other.m_key[&i - &m_key[0]];
	return *this;
}

Block	&Sector::operator[](size_t block)
{
	return m_data[block];
}

void	Sector::setKeyA(uint8_t keyA[6])
{
	Block	tmp;

	tmp = m_data[size() - 1];
	for (size_t i = 0; i < 6; ++i)
		tmp[i] = keyA[i];
	m_state = Sector::MODIFIED;
	if (!m_keyB)
		setAuthentificationKey(keyA);
}

void	Sector::setKeyB(uint8_t keyB[6])
{
	Block	tmp;

	tmp = m_data[size() - 1];
	for (size_t	i = 6; i >= 0; --i)
		tmp[tmp.size() - 1 - i] = keyB[i];
	m_state = Sector::MODIFIED;
	if (m_keyB)
		setAuthentificationKey(keyB);
}

void	Sector::setPermissions(Sector::Flag permissions)
{
	(void)permissions;
}

void	Sector::useKeyB(bool keyB)
{
	m_keyB = keyB;
}

bool	Sector::keyB() const
{
	return m_keyB;
}

void	Sector::setAuthentificationKey(const uint8_t *key)
{
	std::memcpy(m_key.data(), key, m_key.size());
}

const uint8_t	*Sector::authentificationKey() const
{
	return m_key.data();
}

Sector::State	Sector::getState() const
{
	return m_state;
}

bool	Sector::verifyBCC() const
{
	Block	tmp;

	if (!m_isTrailer)
		return true;
	tmp = m_data[0];
	return tmp[0] ^ tmp[1] ^ tmp[2] ^ tmp[3] ^ tmp[4] == 0x00;
}

void	Sector::setTrailer(bool isTrailer)
{
	m_isTrailer = isTrailer;
}

size_t	Sector::size() const
{
	return m_data.size();
}

void	Sector::setState(Sector::State state)
{
	m_state = state;
}

std::vector<Block>::iterator	Sector::begin()
{
	return m_data.begin();
}

std::vector<Block>::iterator	Sector::end()
{
	return m_data.end();
}

std::vector<Block>::const_iterator	Sector::cbegin()
{
	return m_data.cbegin();
}

std::vector<Block>::const_iterator	Sector::cend()
{
	return m_data.cend();
}

