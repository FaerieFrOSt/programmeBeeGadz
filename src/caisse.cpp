#include "caisse.h"
#include <exception>

Caisse::Caisse(Printer *printer, NfcDevice *device, std::function<void(std::string)> &sql, Config *config) :
	Mode(printer, device, sql, config)
{}

Caisse::~Caisse()
{}

void	Caisse::incrementCredit(Card &card, float credit)
{
	if (!credit)
		return;
	credit += getCredit(card);
	std::memcpy(&card[1][1][0], &credit, 4);
	card[1][1].setState(Block::MODIFIED);
	writeCard(card);
}

void	Caisse::setTicket(Card &card, uint8_t id)
{
	card[1][2][0] = 1;
	card[1][2][1] = id;
	card[1][2].setState(Block::MODIFIED);
	writeCard(card);
}

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
					m_printer->printInfo("Posez une carte.");
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
			}
			else if (isConso(*card))
			{
				m_printer->printInfo("Ceci est une carte CONSO");
				m_printer->printInfo("Posez une carte.");
				continue;
			}
			else if (!isDebit(*card))
			{
				m_printer->printInfo("Création de la carte en cours");
				createDebit(*card);
			}
			else if (isDebit(*card))
			{
				m_printer->printInfo("Montant sur la carte : " + Printer::valueToString<float>(getCredit(*card)));
				if (hasTicket(*card))
				{
					try
					{
						m_printer->printInfo("Ticket sur la carte : " + m_config->getConso(getTicket(*card)).first);
					} catch (std::exception &e)
					{}
				}
				m_printer->printInfo("Montant (0) ou ticket (1) (2 pour annuler) ? ");
				int	key = m_printer->getKeyPressed();
				if (key == '1')
				{
					setTicket(*card, std::stoi((*m_config)["ticket"]));
					auto	tmp = m_config->getConso(std::stoi((*m_config)["ticket"]));
					m_printer->printInfo("Ticket " + tmp.first + " ajouté.");
					sendHistory("DEBIT ticket " + tmp.first, tmp.second);
					m_printer->printInfo("Posez une carte");
				}
				else if (key == '0')
				{
					m_printer->printInfo("Entrez le montant à ajouter sur la carte : ");
					float	credit = m_printer->getFloat();
					incrementCredit(*card, credit);
					sendHistory("DEBIT", credit);
					m_printer->printInfo("Montant sur la carte : " + Printer::valueToString<float>(getCredit(*card)));
					m_printer->printInfo("Posez une carte");
				}
				else
					m_printer->printInfo("Posez une carte.");
			}
		} catch (std::exception &e)
		{}
	}
	return false;
}
