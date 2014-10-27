/*************************************/
/*			Implementation of class  */
/*          facility for nfc         */
/*************************************/

#include "nfc_driver.h"
#include <nfc/nfc-types.h>
#include <exception>

nfc_context				*NfcDevice::m_context = nullptr;
int						NfcDevice::m_instances = 0;
const nfc_modulation	NfcDevice::nmMifare = {
	.nmt = NMT_ISO14443A,
	.nbr = NBR_106,
};

NfcDevice::NfcDevice(Printer *print) : m_device(nullptr), m_print(print), m_isInit(false)
{
	if (!m_instances && !m_context)
	{
		m_print->printDebug("Initializing context");
		nfc_init(&m_context);
		if (!m_context)
		{
			m_print->printError("Error initialising the context for NFC");
			throw std::exception();
		}
	}
	m_print->printDebug("Openning device");
	m_device = nfc_open(m_context, nullptr);
	if (!m_device)
	{
		m_print->printError("Error, unable to open NFC device");
		throw std::exception();
	}
	m_print->printDebug("Initializing device");
	if (nfc_initiator_init(m_device) < 0)
	{
		nfc_perror(m_device, "nfc_initiator_init");
		throw std::exception();
	}
	m_print->printDebug("Device name : " + std::string(nfc_device_get_name(m_device)));
	++m_instances;
}

bool    NfcDevice::infiniteSelect(bool infinite)
{
	m_print->printDebug("Setting the device to infinite/once mode");
	if (nfc_device_set_property_bool(m_device, NP_INFINITE_SELECT, infinite) < 0)
	{
		m_print->printError("Error while trying to set property");
		nfc_perror(m_device, "nfc_device_set_property_bool");
		return false;
	}
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

Card	*NfcDevice::findCard()
{
	nfc_target	target;

	infiniteSelect(true);
	m_print->printDebug("Searching for a card...");
	if (nfc_initiator_select_passive_target(m_device, nmMifare, nullptr, 0, &target) > 0)
	{
		m_print->printDebug("Found NFC tag : ");
		m_print->printDebug("UID : " + Printer::arrayToString<uint8_t>(target.nti.nai.abtUid, target.nti.nai.szUidLen));
		m_print->printDebug("Type : " + Printer::arrayToString<uint8_t>(&target.nti.nai.abtAtqa[1], 1));
		Card	*tmp = new Card(this, m_print, target);
		return tmp;
	}
	m_print->printError("Error while searching for a card");
	return nullptr;
}

bool    NfcDevice::mifareCmd(mifare_cmd cmd, size_t sector, mifare_param *param)
{
	return nfc_initiator_mifare_cmd(m_device, cmd, sector, param);
}

bool    NfcDevice::findCard(uint8_t uid[8], size_t len)
{
	static const nfc_modulation nmMifare = {
		.nmt = NMT_ISO14443A,
		.nbr = NBR_106,
	};

	infiniteSelect(false);
	return nfc_initiator_select_passive_target(m_device, nmMifare, uid, len, nullptr) > 0;
}
