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
	while (true)
	{
		std::unique_ptr<Card>	card;
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
			card = m_device->findCard();
			if (!card)
				continue;
			card->readSector(1);
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
