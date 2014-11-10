#include "sector.h"
#include <cstring>
#include <bitset>

const uint16_t	Sector::m_accessTrailer[8] = 
{
	Sector::A_W_A | Sector::A_R_C | Sector::A_R_B | Sector::A_W_B,
	Sector::A_W_A | Sector::A_R_C | Sector::A_W_C | Sector::A_R_B | Sector::A_W_B,
	Sector::A_R_C | Sector::A_R_B,
	Sector::B_W_A | Sector::A_R_C | Sector::B_R_C | Sector::B_W_C | Sector::B_W_B,
	Sector::B_W_A | Sector::A_R_C | Sector::B_R_C | Sector::B_W_B,
	Sector::A_R_C | Sector::B_R_C | Sector::B_W_C,
	Sector::A_R_C | Sector::B_R_C,
	Sector::A_R_C | Sector::B_R_C,
};

const uint8_t	Sector::m_accessBlock[8] =
{
	Sector::A_R | Sector::B_R | Sector::A_W | Sector::B_W | Sector::A_I | Sector::B_I | Sector::A_D | Sector::B_D,
	Sector::A_R | Sector::B_R | Sector::A_D | Sector::B_D,
	Sector::A_R | Sector::B_R,
	Sector::B_R | Sector::B_W,
	Sector::A_R | Sector::B_R | Sector::B_W,
	Sector::B_R,
	Sector::A_R | Sector::B_R | Sector::B_W | Sector::B_I | Sector::A_D | Sector::B_D,
	0,
};

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
	m_state = Sector::MODIFIED;
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
	for (size_t	i = 6; i > 0; --i)
		tmp[tmp.size() - 1 - i] = keyB[i];
	tmp[tmp.size() - 1] = keyB[0];
	m_state = Sector::MODIFIED;
	if (m_keyB)
		setAuthentificationKey(keyB);
}

bool	Sector::setPermissions(size_t block, Sector::Flag permissions)
{
	if (block == m_data.size() - 1 && permissions & Sector::B_I)
		return false;
	else if (block != m_data.size() - 1 && (permissions & Sector::A_W_A || permissions & Sector::B_W_A))
		return false;
	if (block >= m_data.size())
		return false;
	std::vector<uint16_t>	tab;
	if (block == m_data.size() - 1)
	{
		for (auto &i : m_accessTrailer)
			if (i == permissions)
				tab.push_back(&i - &m_accessTrailer[0]);
	}else
	{
		for (auto &i : m_accessBlock)
			if (i == permissions)
				tab.push_back(&i - &m_accessBlock[0]);
	}
	if (tab.size() != 1)
		return false;
	std::bitset<3>	bits(tab[0]);
	Block	*tmp = &m_data[m_data.size() - 1];
	if (m_data.size() > 4)
	{
		if (block < 5)
			block = 0;
		else if (block < 10)
			block = 1;
		else if (block < 15)
			block = 2;
		else block = 3;
	}
	for (size_t i = 6; i < 9; ++i)
	{
		std::bitset<8>	uchar((*tmp)[i]);
		switch (i)
		{
			case 6:
				uchar[block] = ~bits[2];
				uchar[block + 4] = ~bits[1];
				break;
			case 7:
				uchar[block] = ~bits[0];
				uchar[block + 4] = bits[2];
				break;
			case 8:
				uchar[block] = bits[1];
				uchar[block + 4] = bits[0];
				break;
		}
		(*tmp)[i] = (uint8_t)uchar.to_ulong();
	}
	m_state = Sector::MODIFIED;
	return true;
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
	return (tmp[0] ^ tmp[1] ^ tmp[2] ^ tmp[3] ^ tmp[4]) == 0x00;
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

