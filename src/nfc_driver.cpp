/*************************************/
/*			Implementation of class  */
/*          facility for nfc         */
/*************************************/

#include "nfc_driver.h"
#include <nfc/nfc-types.h>
#include <cstring>
#include <sstream>
#include <iomanip>

nfc_context *NfcDevice::m_context = 0;
int NfcDevice::m_instances = 0;

template<typename T>
std::string arrayToString(T array[], size_t len)
{
	std::stringstream  oss("");
	oss << std::hex << std::setfill('0');
	for (size_t tmp = 0; tmp < len; ++tmp)
		oss << std::setw(2) << +array[tmp] << " ";
	return oss.str();
}

NfcDevice::NfcDevice(Printer *print) : m_device(0), m_print(print), m_isInit(false)
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
	m_device = nfc_open(m_context, 0);
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

Card    *NfcDevice::findCard()
{
	static const nfc_modulation nmMifare = {
		.nmt = NMT_ISO14443A,
		.nbr = NBR_106,
	};

	Card    *tmp;
	m_print->printDebug("Searching for a card...");
	if (nfc_initiator_select_passive_target(m_device, nmMifare, 0, 0, &m_target) > 0)
	{
		m_print->printDebug("Found NFC tag : ");
		m_print->printDebug(arrayToString<uint8_t>(m_target.nti.nai.abtUid, m_target.nti.nai.szUidLen));
		m_print->printDebug(arrayToString<uint8_t>(&m_target.nti.nai.abtAtqa[1], 1));
		tmp = new Card(m_target.nti.nai.abtUid, m_target.nti.nai.abtAtqa[1]);
		return tmp;
	}
	m_print->printError("Error while searching for a card");
	return 0;
}
