#include "card.h"
#include <cstring>
#include "nfc_driver.h"
#include <exception>

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

bool	Card::readAllCard()
{
	for (size_t i = 0; i < m_data.size(); ++i)
		if (!readSector(i))
			return false;
	if (m_print->isDebug())
	{
		m_print->printDebug("Data on card " + Printer::arrayToString<uint8_t>(getUid(), m_uidLen) + " :");
		for (auto &sector : m_data)
		{
			m_print->printDebug("Data on sector " + Printer::valueToString<size_t>(&sector - &m_data[0], true) + " :");
			for (auto &i : sector)
			{
				m_print->printDebug("Block " + Printer::valueToString<size_t>(&i - &sector[0], true));
				m_print->printDebug(Printer::arrayToString<uint8_t>(i.data(), i.size()));
			}
		}
	}
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
	return block += m_data[sector].size() - 1;
}

bool	Card::authenticate(size_t sector)
{
	bool            res;
	NfcDevice       *device;

	device = static_cast<NfcDevice*>(m_device);
	m_print->printDebug("Trying to authenticate sector " + Printer::arrayToString<size_t>(&sector, 1) + " on card " + Printer::arrayToString<uint8_t>(getUid(), m_uidLen));
	std::memcpy(m_param.mpa.abtKey, m_data[sector].authentificationKey(), 6);
	m_print->printDebug("Trying with key " + Printer::arrayToString<uint8_t>(m_param.mpa.abtKey, 6) + " on key " + std::string(m_data[sector].keyB() ? "B" : "A"));
	res = device->mifareCmd(m_data[sector].keyB() ? MC_AUTH_B : MC_AUTH_A, calculateBlock(sector), &m_param);
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
	m_print->printError("Error while trying to authenticate sector " + Printer::valueToString<size_t>(sector, true));
	return false;
}

bool    Card::readData(size_t sector)
{
	bool            res;
	NfcDevice       *device;
	size_t			block;
	size_t			tmp;

	device = static_cast<NfcDevice*>(m_device);
	block = calculateBlock(sector) - m_data[sector].size() + 1;
	tmp = 0;
	for (auto &i : m_data[sector])
	{
		if (!(res = device->mifareCmd(MC_READ, block + tmp, &m_param)))
		{
			if (!device->findCard(getUid(), m_uidLen))
			{
				m_print->printError("The tag was removed !");
				return false;
			}
			m_print->printError("Error while reading sector " + Printer::valueToString<size_t>(sector, true));
			return false;
		}else
			std::memcpy(i.data(), m_param.mpd.abtData, i.size());
		++tmp;
	}
	m_data[sector].setState(Sector::CLEAN);
	return true;
}

bool	Card::readMifareClassic(size_t sector)
{
	if (authenticate(sector))
	{
		if (!readData(sector))
		{
			m_print->printError("Error while loading sector " + Printer::valueToString<size_t>(sector, true));
			return false;
		}else
		{
			m_print->printInfo("Tag sector reading completed !\n");
			return true;
		}
	}
	return false;
}

bool    Card::writeSector(size_t sector)
{
	if (!m_writeCard(sector))
		return false;
	return true;
}

bool    Card::writeData(size_t sector)
{
	bool            res;
	NfcDevice       *device;
	size_t			block;
	size_t			tmp;

	device = static_cast<NfcDevice*>(m_device);
	tmp = 0;
	block = calculateBlock(sector) - m_data[sector].size() + 1;
	for (auto &i : m_data[sector])
	{
		std::memcpy(m_param.mpd.abtData, i.data(), i.size());
		if (!(res = device->mifareCmd(MC_WRITE, block + tmp, &m_param)))
		{
			if (!device->findCard(getUid(), m_uidLen))
			{
				m_print->printError("The tag was removed !");
				return false;
			}
			return false;
		}
		++tmp;
	}
	m_data[sector].setState(Sector::CLEAN);
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
	if (m_data[sector].getState() == Sector::DIRTY || m_data[sector].getState() == Sector::CLEAN)
	{
		m_print->printError("Nothing to write to card");
		return false;
	}
	if (!authenticate(sector))
		return false;
	if (!writeData(sector))
	{
		m_print->printError("Error while writing sector " + Printer::valueToString<size_t>(sector, true));
		return false;
	}
	return true;
}
