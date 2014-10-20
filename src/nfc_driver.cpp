/*************************************/
/*			Implementation of class  */
/*          facility for nfc         */
/*************************************/

#include "nfc_driver.h"
#include <nfc/nfc-types.h>

nfc_context *NfcDevice::m_context = nullptr;
int NfcDevice::m_instances = 0;

NfcDevice::NfcDevice(Printer *print) : m_device(nullptr), m_print(print), m_isInit(false)
{
	++m_instances;
}

bool    NfcDevice::init()
{
	if (m_instances == 1 && !m_context)
	{
		m_print->printDebug("Initializing context");
		nfc_init(&m_context);
		if (!m_context)
		{
			m_print->printError("Error initialising the context for NFC");
			return false;
		}
	}
	m_print->printDebug("Openning device");
	m_device = nfc_open(m_context, nullptr);
	if (!m_device)
	{
		m_print->printError("Error, unable to open NFC device");
		return false;
	}
	m_print->printDebug("Initializing device");
	if (nfc_initiator_init(m_device) < 0)
	{
		nfc_perror(m_device, "nfc_initiator_init");
		return false;
	}
	m_print->printDebug(std::string("Device name : ") + std::string(nfc_device_get_name(m_device)));
	return true;
}

NfcDevice::~NfcDevice()
{
	--m_instances;
	if (m_device)
		nfc_close(m_device);
	if (!m_instances)
		nfc_exit(m_context);
}

bool    NfcDevice::findCard()
{
	static const nfc_modulation nmMifare = {
		.nmt = NMT_ISO14443A,
		.nbr = NBR_106,
	};

	m_print->printDebug("Searching for a card...");
	if (nfc_initiator_select_passive_target(m_device, nmMifare, nullptr, 0, &m_target) > 0)
	{
		m_print->printDebug("Found NFC tag : ");
		m_print->printDebug("UID : " + Printer::arrayToString<uint8_t>(m_target.nti.nai.abtUid, m_target.nti.nai.szUidLen));
		m_print->printDebug("Type : " + Printer::arrayToString<uint8_t>(&m_target.nti.nai.abtAtqa[1], 1));
		std::unique_ptr<Card>    tmp(new Card(this, m_print, m_target.nti.nai.abtUid, m_target.nti.nai.abtAtqa[1]));
		if (static_cast<bool>(m_card) && *m_card != *tmp)
			m_card = std::move(tmp);
		else if (!m_card)
			m_card = std::move(tmp);
		return true;
	}
	m_print->printError("Error while searching for a card");
	return false;
}

bool    NfcDevice::mifareCmd(mifare_cmd key, size_t sector, mifare_param *param)
{
	return nfc_initiator_mifare_cmd(m_device, key, sector, param);
}

bool    NfcDevice::readCard()
{
	if (m_card)
		return m_card->read();
	return false;
}
