#include "block.h"

Block::Block(uint8_t *data) : m_state(Block::DIRTY)
{
	if (!data)
	{
		m_data.fill(0x0);
		return;
	}
	for (auto &i : m_data)
		i = data[&i - &m_data[0]];
}

Block::Block(const Block &other) : m_state(other.m_state)
{
	for (auto &i : m_data)
		i = other.m_data[&i - &m_data[0]];
}

Block	&Block::operator=(const Block &other)
{
	m_state = other.m_state;
	for (auto &i : m_data)
		i = other.m_data[&i - &m_data[0]];
	return *this;
}

uint8_t	&Block::operator[](size_t block)
{
	m_state = Block::MODIFIED;
	return m_data[block];
}

std::array<uint8_t, 16>::iterator	Block::begin()
{
	return m_data.begin();
}

std::array<uint8_t, 16>::iterator	Block::end()
{
	return m_data.end();
}

std::array<uint8_t, 16>::const_iterator	Block::cbegin()
{
	return m_data.cbegin();
}

std::array<uint8_t, 16>::const_iterator	Block::cend()
{
	return m_data.cend();
}

uint8_t	*Block::data()
{
	return m_data.data();
}

size_t	Block::size() const
{
	return m_data.size();
}

Block::State	Block::getState() const
{
	return m_state;
}

void	Block::setState(Block::State state)
{
	m_state = state;
}
