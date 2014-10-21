#include "card.h"
#include <cstring>
#include "mifare.h"
#include "nfc_driver.h"

const uint8_t   Card::keys[] =
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

Card::Card(void *device, Printer *print, uint8_t uid[8], uint8_t type) : m_type(type), m_print(print), m_device(device)
{
	for (int i = 0; i < 16; ++i)
		m_sectorState[i] = Card::DIRTY;
	std::memcpy(&m_uid, uid, 4);
}

Card::~Card()
{}

bool    Card::operator==(Card &other)
{
	return m_uid == other.m_uid;
}

bool    Card::operator!=(Card &other)
{
	return m_uid != other.m_uid;
}

bool    Card::read(size_t sector)
{
	std::memset(m_data, 0, sizeof(uint8_t) * 1024);
	if (m_type == Card::MifareClassic)
	{
		if (!loadMifare(sector))
			return false;
	}else
	{
		m_print->printError("Not a supported card yet!");
		return false;
	}
	m_print->printDebug("Data on card :");
	if (sector == 666)
		for (size_t i = 0; i < 16; ++i)
		{
			m_print->printDebug("Sector " + Printer::arrayToString<size_t>(&i, 1) + ":");
			for (size_t j = 0; j < 4; ++j)
			{
				m_print->printDebug("Block " + Printer::arrayToString<size_t>(&j, 1) + " : " + Printer::arrayToString<uint8_t>(m_data[i][j], 16));
			}
		}
	else
	{
		m_print->printDebug("Sector " + Printer::arrayToString<size_t>(&sector, 1) + ":");
		for (size_t j = 0; j < 4; ++j)
		{
			m_print->printDebug("Block " + Printer::arrayToString<size_t>(&j, 1) + " : " + Printer::arrayToString<uint8_t>(m_data[sector][j], 16));
		}
	}
	return true;
}

bool    Card::authenticate(size_t s)
{
	mifare_param    mp;
	bool            res;
	NfcDevice       *device;
	bool            keyB;

	keyB = false;
	device = static_cast<NfcDevice*>(m_device);
	std::memcpy(mp.mpa.abtAuthUid, &m_uid, 4);
	m_print->printDebug("Trying to authenticate sector " + Printer::arrayToString<size_t>(&s, 1) + " on card " + Printer::arrayToString<uint8_t>((uint8_t*)&m_uid, 4));
	for (size_t i = 0; i < sizeof(Card::keys) / (sizeof(uint8_t) * 6); i += 6)
	{
		std::memcpy(mp.mpa.abtKey, &Card::keys[i], 6);
		res = device->mifareCmd(keyB ? MC_AUTH_B : MC_AUTH_A, s * 4 + 3, &mp);
		if (res)
		{
			m_print->printDebug("Authenticated normaly");
			return true;
		}
		else if (!device->findCard((uint8_t*)&m_uid, 4))
		{
			m_print->printError("The tag was removed !");
			device->deleteCard();
			return false;
		}
	}
	m_print->printError("Error while trying to authenticate sector " + Printer::arrayToString<size_t>(&s, 1));
	return false;
}

bool    Card::readData(size_t s)
{
	bool            res;
	NfcDevice       *device;
	mifare_param    mp;

	std::memcpy(mp.mpa.abtAuthUid, &m_uid, 4);
	std::memcpy(mp.mpa.abtKey, &Card::keys[0], 6);
	device = static_cast<NfcDevice*>(m_device);
	for (size_t i = 0; i < 4; ++i)
	{
		if (!(res = device->mifareCmd(MC_READ, s * 4 + i, &mp)))
		{
			if (!device->findCard((uint8_t*)&m_uid, 4))
			{
				m_print->printError("The tag was removed !");
				device->deleteCard();
				return false;
			}
			return false;
		}else
			std::memcpy(m_data[s][i], mp.mpd.abtData, 16);
	}
	m_sectorState[s] = Card::CLEAN;
	return true;
}

bool    Card::loadMifare(size_t sector)
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
	if (len >= 16 * 3)
		return false;
	if (sector == 0 || sector >= 16)
		return false;
	if (m_sectorState[sector] != Card::CLEAN)
		if (!read(sector))
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
	mifare_param    mp;

	std::memset(mp.mpd.abtData, 0, 16);
	std::memcpy(mp.mpa.abtAuthUid, &m_uid, 4);
	std::memcpy(mp.mpa.abtKey, &Card::keys[0], 6);
	device = static_cast<NfcDevice*>(m_device);
	for (size_t i = 0; i < 4; ++i)
	{
		std::memcpy(mp.mpd.abtData, m_data[sector][i], 16);
		if (!(res = device->mifareCmd(MC_WRITE, sector * 4 + i, &mp)))
		{
			if (!device->findCard((uint8_t*)&m_uid, 4))
			{
				m_print->printError("The tag was removed !");
				device->deleteCard();
				return false;
			}
			return false;
		}
	}
	m_sectorState[sector] = Card::DIRTY;
	return true;
}

bool    Card::write()
{
	for (size_t i = 0; i < 16; ++i)
	{
		if (m_sectorState[i] == Card::DIRTY || m_sectorState[i] == Card::CLEAN)
			continue;
		if (!authenticate(i))
			return false;
		if (!writeData(i))
		{
			m_print->printError("Error while writing sector " + Printer::arrayToString<size_t>(&i, 1));
			return false;
		}
	}
	return true;
}
