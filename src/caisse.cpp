#include "caisse.h"
#include <exception>
#include <ncurses.h>

Caisse::Caisse(Printer *printer, NfcDevice *device, Mysql *sql, Config *config) :
	Mode(printer, device, sql, config)
{}

Caisse::~Caisse()
{}

bool	Caisse::run()
{
	m_printer->printInfo("Mode Caisse");
	std::unique_ptr<Card>	card(nullptr);
	bool					read = false;
	m_printer->printInfo("Posez une carte");
	while (true)
	{
		try
		{
			if (m_printer->keyPressed())
				return false;
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
				if (!card->readSector(1))
					continue;
				read = true;
				m_printer->printInfo("Lecture terminée");
			}
			if (!isDebit(*card))
				createDebit(*card);
			if (isDebit(*card))
			{
				m_printer->printInfo("Montant (0) ou ticket (1) ? ");
				if (m_printer->getKeyPressed() == '1')
				{
					setTicket(*card, (*m_config)["Ticket"]);
					m_printer->printInfo("Ticket " + (*m_config)["Ticket"] + " ajouté.");
				}
				else
				{
					m_printer->printInfo("Montant sur la carte : " + Printer::valueToString<float>(getCredit(*card)));
					m_printer->printInfo("Entrez le montant à ajouter sur la carte : ");
					float	credit = m_printer->getFloat();
					incrementCredit(*card, credit);
					m_printer->printInfo("Montant sur la carte : " + Printer::valueToString<float>(getCredit(*card)));
				}
			}
		} catch (std::exception &e)
		{}
	}
	return false;
}
