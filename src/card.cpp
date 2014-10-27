#include "card.h"
#include <cstring>
#include "nfc_driver.h"
#include <exception>

const uint8_t   Card::m_keys[] =
{
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xd3, 0xf7, 0xd3, 0xf7, 0xd3, 0xf7,
	0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5,
	0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5,
	0x4d, 0x3a, 0x99, 0xc3, 0x51, 0xdd,
	0x1a, 0x98, 0x2c, 0x7e, 0x45, 0x9a,
	0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
	0xab, 0xcd, 0xef, 0x12, 0x34, 0x56,
};

Card::Card(void *device, Printer *print, nfc_target target) : m_print(print), m_device(device), m_keyB(false)
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
			m_nbSectors = 16;
			m_sectorLen = 4;
			m_uidLen = 4;
			break;

		default:
			m_print->printError("Not a reconized card !");
			throw std::exception();
	}
	m_sectorState = new State[m_nbSectors];
	m_data = new uint8_t**[m_nbSectors];
	for (size_t i = 0; i < m_nbSectors; ++i)
	{
		m_sectorState[i] = Card::DIRTY;
		m_data[i] = new uint8_t*[m_sectorLen];
		for (size_t j = 0; j < m_sectorLen; ++j)
			m_data[i][j] = new uint8_t[sizeBlock];
	}
	std::memcpy(m_param.mpa.abtAuthUid, m_target.nti.nai.abtUid, m_uidLen);
}

Card::~Card()
{
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
	return !operator==(other);
}

bool	Card::read()
{
	return read(666);
}

bool    Card::read(size_t sector)
{
	if (!m_readCard(sector))
		return false;
	if (m_print->isDebug())
	{
		m_print->printDebug("Data on card :");
		if (sector == 666)
			for (size_t i = 0; i < m_nbSectors; ++i)
			{
				m_print->printDebug("Sector " + Printer::arrayToString<size_t>(&i, 1) + ":");
				for (size_t j = 0; j < m_sectorLen; ++j)
				{
					m_print->printDebug("Block " + Printer::arrayToString<size_t>(&j, 1) + " : " + Printer::arrayToString<uint8_t>(m_data[i][j], sizeBlock));
				}
			}
		else
		{
			m_print->printDebug("Sector " + Printer::arrayToString<size_t>(&sector, 1) + ":");
			for (size_t j = 0; j < m_sectorLen; ++j)
			{
				m_print->printDebug("Block " + Printer::arrayToString<size_t>(&j, 1) + " : " + Printer::arrayToString<uint8_t>(m_data[sector][j], sizeBlock));
			}
		}
	}	
	return true;
}

void	Card::setKeyB(bool keyB)
{
	m_keyB = keyB;
}

uint8_t	*Card::getUid()
{
	return m_target.nti.nai.abtUid;
}

bool	Card::authenticate(size_t sector)
{
	bool            res;
	NfcDevice       *device;

	device = static_cast<NfcDevice*>(m_device);
	m_print->printDebug("Trying to authenticate sector " + Printer::arrayToString<size_t>(&sector, 1) + " on card " + Printer::arrayToString<uint8_t>(getUid(), m_uidLen));
	for (size_t i = 0; i < sizeof(Card::m_keys) / (sizeof(uint8_t) * 6); i += 6)
	{
		std::memcpy(m_param.mpa.abtKey, &Card::m_keys[i], 6);
		m_print->printDebug("Trying with key " + Printer::arrayToString<uint8_t>(m_param.mpa.abtKey, 6) + " on key " + std::string(m_keyB ? "B" : "A"));
		res = device->mifareCmd(m_keyB ? MC_AUTH_B : MC_AUTH_A, sector * m_sectorLen + m_sectorLen - 1, &m_param);
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
	}
	m_print->printError("Error while trying to authenticate sector " + Printer::arrayToString<size_t>(&sector, 1));
	return false;
}

bool    Card::readData(size_t sector)
{
	bool            res;
	NfcDevice       *device;

	/* std::memcpy(m_param.mpa.abtKey, &Card::keys[0], 6); */
	device = static_cast<NfcDevice*>(m_device);
	for (size_t i = 0; i < m_sectorLen; ++i)
	{
		if (!(res = device->mifareCmd(MC_READ, sector * m_sectorLen + i, &m_param)))
		{
			if (!device->findCard(getUid(), 4))
			{
				m_print->printError("The tag was removed !");
				return false;
			}
			m_print->printError("Error while reading sector " + Printer::arrayToString<size_t>(&sector, 1));
			return false;
		}else
			std::memcpy(m_data[sector][i], m_param.mpd.abtData, sizeBlock);
	}
	m_sectorState[sector] = Card::CLEAN;
	return true;
}

bool    Card::readMifareClassic(size_t sector)
{
	if (sector != 666)
	{
		if (authenticate(sector))
		{
			if (!readData(sector))
			{
				m_print->printError("Error while loading sector " + Printer::arrayToString<size_t>(&sector, 1));
				return false;
			}
			else
			{
				m_print->printInfo("Tag reading completed !\n");
				return true;
			}
		}else
			return false;
	}
	for (size_t i = 0; i < 16; ++i)
	{
		if (authenticate(i))
		{
			if (!readData(i))
			{
				m_print->printError("Error while loading sector " + Printer::arrayToString<size_t>(&i, 1));
				return false;
			}
		}else
			return false;
	}
	m_print->printInfo("Tag reading completed !\n");
	return true;
}

bool    Card::write(size_t sector, uint8_t *data, size_t len)
{
	if (len >= sizeBlock * (m_sectorLen - 1))
		return false;
	if (sector == 0 || sector >= m_nbSectors)
		return false;
	if (m_sectorState[sector] != Card::CLEAN)
		if (!m_readCard(sector))
			return false;
	std::memcpy(m_data[sector], data, len);
	m_sectorState[sector] = Card::MODIFIED;
	m_print->printDebug("Wrote in array");
	return true;
}

bool    Card::writeData(size_t sector)
{
	bool            res;
	NfcDevice       *device;

	/* std::memcpy(mp.mpa.abtKey, &Card::keys[0], 6); */
	device = static_cast<NfcDevice*>(m_device);
	for (size_t i = 0; i < 4; ++i)
	{
		std::memcpy(m_param.mpd.abtData, m_data[sector][i], sizeBlock);
		if (!(res = device->mifareCmd(MC_WRITE, sector * m_sectorLen + i, &m_param)))
		{
			if (!device->findCard(getUid(), 4))
			{
				m_print->printError("The tag was removed !");
				return false;
			}
			return false;
		}
	}
	m_sectorState[sector] = Card::DIRTY;
	return true;
}

bool	Card::write()
{
	for (size_t i = 0; i < m_nbSectors; ++i)
		if (!m_writeCard(i))
			return false;
	return true;
}

bool    Card::writeMifareClassic(size_t sector)
{
	if (m_sectorState[sector] == Card::DIRTY || m_sectorState[sector] == Card::CLEAN)
		return false;
	if (!authenticate(sector))
		return false;
	if (!writeData(sector))
	{
		m_print->printError("Error while writing sector " + Printer::arrayToString<size_t>(&sector, 1));
		return false;
	}
	return true;
}
