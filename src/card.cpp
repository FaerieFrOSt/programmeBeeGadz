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
};

Card::Card(void *device, Printer *print, uint8_t uid[8], uint8_t type) : m_type(type), m_print(print), m_device(device)
{
	std::memcpy(&m_uid, uid, 8);
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

bool    Card::read()
{
	std::memset(m_data, 0, sizeof(block) * 16 * 4);
	if (m_type == Card::MifareClassic)
	{
		if (!loadMifare())
			return false;
	}else
		return false;
	m_print->printDebug("Data on card :");
	for (size_t i = 0; i < 16; ++i)
		m_print->printDebug(Printer::arrayToString<block>(m_data[i], 4, 4));
	return false;
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
	return res;
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
		}
	}
	return true;
}

bool    Card::loadMifare()
{
	for (size_t i = 0; i < sizeof(m_data) / sizeof(sector); ++i)
	{
		if (authenticate(i))
			if (!readData(i))
				m_print->printError("Error while loading sector " + Printer::arrayToString<size_t>(&i, 1));
	}
	return false;	
}
