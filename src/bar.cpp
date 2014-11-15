#include "bar.h"
#include <exception>
#include <memory>
#include <ctime>

Bar::Bar(Printer *printer, NfcDevice *device, Mysql *sql) : Mode(printer, device, sql)
{}

Bar::~Bar()
{}

bool	Bar::run()
{
	std::string	conso = "";
	float		price = 0.0f;
	std::time_t	time = std::time(nullptr);
	m_printer->printInfo("Mode bar is running");
	m_printer->printInfo("Posez une carte.");
	std::unique_ptr<Card>	card;
	while (true)
	{
		try
		{
			std::time_t	timer = std::time(nullptr);
			if (conso != "" && timer - time >= 10 * 60)
			{
				conso = "";
				price = 0.0f;
				m_printer->printInfo("Time ellapsed, Command reset.");
				time = timer;
			}
			if (m_printer->keyPressed())
				return false;
			if (!card)
				card = m_device->findCard(false);
			else if (!m_device->findCard(card->getUid(), card->sizeUid()))
			{
				if (!isConso(*card))
				{
					conso = "";
					price = 0.0f;
				}
				card = m_device->findCard(false);
			}
			if (!card)
				continue;
			m_printer->printInfo("Lecture de la carte en cours, ne pas retirer la carte.");
			card->readSector(1);
			m_printer->printInfo("Lecture terminée");
			if (isAdmin(*card))
				return true;
			else if (isSOS(*card))
				sendSOS();
			else if (isDebit(*card) && conso == "")
				m_printer->printInfo("Entrez d'abord la commande");
			else if (isDebit(*card) && conso != "")
			{
				if (hasTicket(*card) && getTicket(*card) == conso)
				{
					m_printer->printInfo("Ticket " + conso + " utilisé.");
					decrementTicket(*card);
				}
				else if (getCredit(*card) >= price)
				{
					m_printer->printInfo("OK");
					decrementCredit(*card, price);
				}
				else
					m_printer->printInfo("Pas assez d'argent sur la carte !");
				conso = "";
				price = 0.0f;
			}
			else if (isConso(*card))
			{
				//TODO
				m_printer->printInfo("Carte conso.");
			}
		} catch (std::exception &e)
		{}
	}
}
