#include "kve.h"
#include <exception>
#include <ctime>
#include <unordered_map>
#include <functional>

Kve::Kve(Printer *printer, NfcDevice *device, std::function<void(std::string)> &sql, Config *config) : Bar(printer, device, sql, config)
{}

Kve::~Kve()
{}

bool	Kve::run()
{
	m_printer->printInfo("Mode Kve");
	std::unique_ptr<Card>	card(nullptr);
	bool					read = false;
	bool					ticket = false;
	std::time_t				time = std::time(nullptr);
	m_printer->printInfo("Posez une carte DEBIT.");
	while (true)
	{
		try
		{
			std::time_t	timer = std::time(nullptr);
			int	tmp = m_printer->getKeyPressedNB();
			if (tmp == '-' && ticket)
			{
				ticket = false;
				m_printer->printInfo("Ticket annulé.");
			}
			if (ticket && timer - time >= 10)
			{
				ticket = false;
				m_printer->printInfo("Ticket annulé.");
			}
			if (tmp == '0')
			{
				ticket = true;
				m_printer->printInfo("Ticket ajouté.");
				time = timer;
			}
			if (!card)
			{
				read = false;
				card = m_device->findCard(m_config->getKeys(false), false);
			}
			else if (!m_device->findCard(card->getUid(), card->sizeUid()))
			{
				read = false;
				card = m_device->findCard(m_config->getKeys(false), false);
			}
			else
				continue;
			if (!card)
				continue;
			if (!read)
			{
				m_printer->printInfo("Lecture de la carte en cours, ne pas retirer la carte.");
				try
				{
					if (!card->readSector(1))
						continue;
				} catch (std::exception &e)
				{
					if (ticket)
					{
						m_printer->printInfo("Ticket annulé.");
						ticket = false;
					}
					m_printer->printInfo("Posez une carte DEBIT.");
					continue;
				}
				read = true;
				m_printer->printInfo("Lecture terminée");
			}
			if (isAdmin(*card))
				return true;
			else if (isSOS(*card))
			{
				sendSOS();
				m_printer->printInfo("SOS envoyé");
				if (ticket)
				{
					m_printer->printInfo("Ticket annulé.");
					ticket = false;
				}
			}
			else if (isConso(*card))
			{
				m_printer->printInfo("Ceci est une carte CONSO");
				m_printer->printInfo("Posez une carte DEBIT.");
				if (ticket)
				{
					m_printer->printInfo("Ticket annulé.");
					ticket = false;
				}
				continue;
			}
			else if (isDebit(*card))
			{
				if (ticket && hasTicket(*card))
				{
					ticket = false;
					m_printer->printInfo("TIcket utilisé");
					decrementTicket(*card);
					sendHistory("ticket " + m_config->getConso(getTicket(*card)).first, m_config->getConso(getTicket(*card)).second);
					continue;
				}
				m_printer->printInfo("Entrez le prix du cok'ss : ");
				float	credit = m_printer->getFloat();
				if (getCredit(*card) >= credit)
				{
					m_printer->printInfo("OK");
					decrementCredit(*card, credit);
					sendHistory("KVE", credit);
				}
				else
					m_printer->printInfo("Pas assez d'argent sur la carte !");
				if (ticket)
				{
					m_printer->printInfo("Ticket annulé.");
					ticket = false;
				}
			}
			else
				m_printer->printInfo("Pas assez d'argent sur la carte !");
			m_printer->printInfo("Posez une cart DEBIT.");
		} catch (std::exception &e)
		{}
	}
	return false;
}
