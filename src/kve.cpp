#include "kve.h"
#include <exception>
#include <ctime>
#include <unordered_map>
#include <functional>

Kve::Kve(Printer *printer, NfcDevice *device, std::function<void(std::string&)> &sql, Config *config) : Bar(printer, device, sql, config)
{}

Kve::~Kve()
{}

bool	Kve::run()
{
	m_printer->printInfo("Mode Kve");
	std::unique_ptr<Card>	card(nullptr);
	bool					read = false;
	m_printer->printInfo("Posez une carte DEBIT.");
	while (true)
	{
		try
		{
			if (!card)
			{
				read = false;
				card = m_device->findCard(false);
			}
			else if (!m_device->findCard(card->getUid(), card->sizeUid()))
			{
				read = false;
				card = m_device->findCard(false);
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
					m_printer->printInfo("Posez une carte DEBIT.");
					continue;
				}
				read = true;
				m_printer->printInfo("Lecture terminée");
			}
			if (isAdmin(*card))
				return true;
			else if (isSOS(*card))
				sendSOS();
			else if (isConso(*card))
			{
				m_printer->printInfo("Ceci est une carte CONSO");
				m_printer->printInfo("Posez une carte DEBIT.");
				continue;
			}
			else if (isDebit(*card))
			{
				m_printer->printInfo("Entrez le prix du cok'ss : ");
				float	credit = m_printer->getFloat();
				if (getCredit(*card) >= credit)
					decrementCredit(*card, credit);
				else
					m_printer->printInfo("Pas assez d'argent sur la carte !");
				m_printer->printInfo("Posez une cart DEBIT.");
			}
		} catch (std::exception &e)
		{}
	}
	return false;
}
