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
	m_printer->printInfo("Posez une carte");
	std::unique_ptr<Card>	card(nullptr);
	bool					read = false;
	while (true);
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
				card->readSector(1);
				read = true;
				m_printer->printInfo("Lecture terminée");
			}
			if (isDebit(*card))
			{
				m_printer->printInfo("Entrez le montant à ajouter sur la carte");
				int	key;
				do
				{
					key
				} while (key != KEY_ENTER);
			}
		} catch (std::exception &e)
		{}
	}
	return false;
}
