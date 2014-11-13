#include "card.h"
#include <cstring>
#include "nfc_driver.h"
#include <exception>

const uint8_t	Card::m_keys[] = 
{
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xd3, 0xf7, 0xd3, 0xf7, 0xd3, 0xf7,
	0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5,
	0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5,
	0x4d, 0x3a, 0x99, 0xc3, 0x51, 0xdd,
	0x1a, 0x98, 0x2c, 0x7e, 0x45, 0x9a,
	0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xab, 0xcd, 0xed, 0x12, 0x34, 0x56,
};
const size_t	Card::m_nbKeys = 9;

Card::Card(void *device, Printer *print, nfc_target target) : m_print(print), m_device(device)
{
	std::memcpy(&m_target, &target, sizeof(m_target));
	if (!m_print || !m_device)
	{
		throw std::exception();
	}
	switch (m_target.nti.nai.abtAtqa[1])
	{
		case	Card::MifareClassic:
			m_print->printDebug("Reconized a Mifare Classic card");
			m_type = Card::MifareClassic;
			m_readCard = std::bind(&Card::readMifareClassic, this, std::placeholders::_1);
			m_writeCard = std::bind(&Card::writeMifareClassic, this, std::placeholders::_1);
			m_data.resize(16, Sector(false, 4));
			m_data[0].setTrailer(true);
			m_uidLen = 4;
			break;

		default:
			m_print->printError("Not a reconized card !");
			throw std::exception();
	}
	m_data.shrink_to_fit();
	std::memcpy(m_param.mpa.abtAuthUid, m_target.nti.nai.abtUid, m_uidLen);
}

Card::Card(const Card &other) : m_print(other.m_print), m_device(other.m_device)

{
	std::memcpy(&m_target, &other.m_target, sizeof(m_target));
	if (!m_print || !m_device)
	{
		throw std::exception();
	}
	switch (m_target.nti.nai.abtAtqa[1])
	{
		case	Card::MifareClassic:
			m_print->printDebug("Reconized a Mifare Classic card");
			m_type = Card::MifareClassic;
			m_readCard = std::bind(&Card::readMifareClassic, this, std::placeholders::_1);
			m_writeCard = std::bind(&Card::writeMifareClassic, this, std::placeholders::_1);
			m_data.resize(16, Sector(false, 4));
			m_data[0].setTrailer(true);
			m_uidLen = 4;
			break;

		default:
			m_print->printError("Not a reconized card !");
			throw std::exception();
	}
	m_data.shrink_to_fit();
	std::memcpy(m_param.mpa.abtAuthUid, m_target.nti.nai.abtUid, m_uidLen);
}

Card&	Card::operator=(const Card &other)
{
	m_print = other.m_print;
	m_device = other.m_device;
	std::memcpy(&m_target, &other.m_target, sizeof(m_target));
	if (!m_print || !m_device)
	{
		throw std::exception();
	}
	switch (m_target.nti.nai.abtAtqa[1])
	{
		case	Card::MifareClassic:
			m_print->printDebug("Reconized a Mifare Classic card");
			m_type = Card::MifareClassic;
			m_readCard = std::bind(&Card::readMifareClassic, this, std::placeholders::_1);
			m_writeCard = std::bind(&Card::writeMifareClassic, this, std::placeholders::_1);
			m_data.resize(16, Sector(false, 4));
			m_data[0].setTrailer(true);
			m_uidLen = 4;
			break;

		default:
			m_print->printError("Not a reconized card !");
			throw std::exception();
	}
	m_data.shrink_to_fit();
	std::memcpy(m_param.mpa.abtAuthUid, m_target.nti.nai.abtUid, m_uidLen);
	return *this;
}

Sector	&Card::operator[](size_t sector)
{
	return m_data[sector];
}

bool    Card::operator==(Card &other)
{
	for (size_t i = 0; i < m_uidLen; ++i)
		if (m_target.nti.nai.abtUid[i] != other.m_target.nti.nai.abtUid[i])
			return false;
	return true;
}

bool    Card::operator!=(Card &other)
{
	return !(*this == other);
}

size_t	Card::sizeUid() const
{
	return m_uidLen;
}

bool	Card::readAllCard()
{
	for (size_t i = 0; i < m_data.size(); ++i)
		if (!readSector(i))
			return false;
	return true;
}

bool    Card::readSector(size_t sector)
{
	if (!m_readCard(sector))
		return false;
	if (m_print->isDebug())
	{
		m_print->printDebug("Data on sector " + Printer::valueToString<size_t>(sector, true) + " :");
		for (auto &i : m_data[sector])
		{
			m_print->printDebug("Block " + Printer::valueToString<size_t>(&i - &m_data[sector][0], true));
			m_print->printDebug(Printer::arrayToString<uint8_t>(i.data(), i.size()));
		}
	}	
	return true;
}

const uint8_t	*Card::getUid() const
{
	return m_target.nti.nai.abtUid;
}

size_t	Card::calculateBlock(size_t sector) const
{
	size_t	block = 0;
	size_t	i = 0;
	while (i++ != sector)
		block += m_data[i].size();
	return block + m_data[sector].size() - 1;
}

size_t	Card::calculateSector(size_t block, size_t *nb) const
{
	size_t	sector = 0;
	int		b = (int)block;
	while (b - (int)m_data[sector].size() >= 0)
		b -= m_data[sector++].size();
	if (nb)
		*nb = b;
	return sector;
}

bool	Card::authenticate(size_t block)
{
	bool            res;
	NfcDevice       *device;

	device = static_cast<NfcDevice*>(m_device);
	std::memcpy(m_param.mpa.abtAuthUid, getUid(), m_uidLen);
	m_print->printDebug("Trying to authenticate block " + Printer::valueToString<size_t>(block, true) + " on card " + Printer::arrayToString<uint8_t>(m_param.mpa.abtAuthUid, m_uidLen));
	size_t	i = 0;
	size_t	sector = calculateSector(block);
	do
	{
		std::memcpy(m_param.mpa.abtKey, m_data[sector].authentificationKey(), 6);
		m_print->printDebug("Trying with key " + Printer::arrayToString<uint8_t>(m_param.mpa.abtKey, 6) + " on key " + std::string(m_data[sector].keyB() ? "B" : "A"));
		res = device->mifareCmd(m_data[sector].keyB() ? MC_AUTH_B : MC_AUTH_A, block, &m_param);
		if (res)
		{
			m_print->printDebug("Authenticated normaly");
			return true;
		}
		else if (!device->findCard(getUid(), m_uidLen))
		{
			m_print->printError("The tag was removed !");
			return false;
		}
		m_data[sector].setAuthentificationKey(&m_keys[i]);
		i += 6;
	}while (i < m_nbKeys * 6);
	m_print->printError("Error while trying to authenticate block " + Printer::valueToString<size_t>(block, true));
	return false;
}

bool    Card::readData(size_t block)
{
	bool		res;
	NfcDevice	*device;
	size_t		sector;
	size_t		tmp;

	sector = calculateSector(block, &tmp);
	device = static_cast<NfcDevice*>(m_device);
	std::memcpy(m_param.mpa.abtAuthUid, getUid(), m_uidLen);
	if (!(res = device->mifareCmd(MC_READ, block, &m_param)))
	{
		if (!device->findCard(getUid(), m_uidLen))
		{
			m_print->printError("The tag was removed !");
			return false;
		}
		m_print->printError("Error while reading block " + Printer::valueToString<size_t>(block, true));
		return false;
	}else
		std::memcpy(m_data[sector][tmp].data(), m_param.mpd.abtData, m_data[sector][tmp].size());
	m_data[sector][tmp].setState(Block::CLEAN);
	return true;
}

bool	Card::readMifareClassic(size_t sector)
{
	size_t	block = calculateBlock(sector) - m_data[sector].size() + 1;
	for (auto i : m_data[sector])
	{
		if (authenticate(block))
			if (!readData(block))
				m_print->printError("Error while loading block " + Printer::valueToString<size_t>(block, true));
		++block;
		(void)i;
	}
	return true;
}

bool    Card::writeSector(size_t sector)
{
	return m_writeCard(sector);
}

bool    Card::writeData(size_t block)
{
	bool            res;
	NfcDevice       *device;
	size_t			sector;
	size_t			tmp;

	sector = calculateSector(block, &tmp);
	device = static_cast<NfcDevice*>(m_device);
	std::memcpy(m_param.mpd.abtData, m_data[sector][tmp].data(), m_data[sector][tmp].size());
	m_print->printDebug("Trying to write " + Printer::arrayToString<uint8_t>(m_param.mpd.abtData, 16) + " on block " + Printer::valueToString<size_t>(block, true));
	if (!(res = device->mifareCmd(MC_WRITE, block, &m_param)))
	{
		if (!device->findCard(getUid(), m_uidLen))
		{
			m_print->printError("The tag was removed !");
			return false;
		}
		return false;
	}
	return true;
}

bool	Card::writeAllCard()
{
	for (size_t i = 0; i < m_data.size(); ++i)
		if (!m_writeCard(i))
			return false;
	return true;
}

bool    Card::writeMifareClassic(size_t sector)
{
	size_t	block = calculateBlock(sector) - m_data[sector].size() + 1;
	for (auto i : m_data[sector])
	{
		if (i.getState() == Block::CLEAN)
		{
			m_print->printDebug("Nothing to write on block " + Printer::valueToString<size_t>(block, true));
			++block;
			continue;
		}
		if (authenticate(block))
			if (!writeData(block))
				m_print->printError("Error while writing block " + Printer::valueToString<size_t>(block, true));
		++block;
		(void)i;
	}
	return true;
}
