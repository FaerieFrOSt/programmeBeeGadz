#include "admin.h"
#include <memory>
#include <string>

Admin::Admin(Printer *printer, NfcDevice *device, std::function<void(std::string&)> &sql, Config *config) :
	Mode(printer, device, sql, config)
{}

Admin::~Admin()
{}

void	Admin::createConso(Card &card, uint8_t id)
{
	std::string	name("CONSO");
	for (size_t i = 0; i < name.size(); ++i)
		card[1][0][i] = name[i];
	card[1][0][name.size()] = 0;
	card[1][1][0] = id;
	card[1][0].setState(Block::MODIFIED);
	card[1][1].setState(Block::MODIFIED);
	writeCard(card);
}

void	Admin::createAdmin(Card &card)
{
	std::string	name("ADMIN");
	for (size_t i = 0; i < name.size(); ++i)
		card[1][0][i] = name[i];
	card[1][0][name.size()] = 0;
	card[1][0].setState(Block::MODIFIED);
	writeCard(card);
}

void	Admin::createSOS(Card &card)
{
	std::string	name("SOS");
	for (size_t i = 0; i < name.size(); ++i)
		card[1][0][i] = name[i];
	card[1][0][name.size()] = 0;
	card[1][0].setState(Block::MODIFIED);
	writeCard(card);
}

bool	Admin::run()
{
	m_printer->printInfo("Mode admin");
	bool end;
	while (true)
	{
		m_printer->printInfo("--- Menu ---");
		m_printer->printInfo("0 - quit Admin mode");
		m_printer->printInfo("1 - quit program");
		m_printer->printInfo("2 - set mode");
		m_printer->printInfo("3 - create card");
		switch (m_printer->getKeyPressed())
		{
			case '3':
				m_printer->clearScreen();
				end = false;
				while (!end)
				{
					m_printer->printInfo("--- Menu ---");
					m_printer->printInfo("0 - create ADMIN card");
					m_printer->printInfo("1 - create CONSO card");
					m_printer->printInfo("2 - create DEBIT card");
					m_printer->printInfo("3 - create SOS card");
					m_printer->printInfo("Any other key to return to the precedent menu");
					std::unique_ptr<Card>	card(nullptr);
					int	tmp;
					switch (m_printer->getKeyPressed())
					{
						case '0':
							m_printer->printInfo("Posez une carte");
							card = m_device->findCard(true);
							createAdmin(*card);
							break;
						case '1':
							m_printer->clearScreen();
							m_printer->printInfo("--- Menu ---");
							for (size_t i = 0; i < m_config->getNbConso(); ++i)
								m_printer->printInfo(Printer::valueToString<size_t>(i) + " - " + m_config->getConso(i).first);
							m_printer->printInfo("Entrez votre choix de conso : ");
							tmp = m_printer->getKeyPressed();
							tmp -= '0';
							if (tmp < 0 || (size_t)tmp > m_config->getNbConso())
							{
								m_printer->printError("Mauvaise conso, appuyez sur une touche");
								while (!m_printer->keyPressed());
								m_printer->clearScreen();
								break;
							}
							m_printer->printInfo("Posez une carte");
							card = m_device->findCard(true);
							createConso(*card, tmp);
							break;
						case '2':
							m_printer->printInfo("Posez une carte");
							card = m_device->findCard(true);
							createDebit(*card);
							break;
						case '3':
							m_printer->printInfo("Posez une carte");
							card = m_device->findCard(true);
							createSOS(*card);
							break;
						default:
							end = true;
							break;
					}
				}
				m_printer->clearScreen();
				break;
			case '2':
				m_printer->clearScreen();
				end = false;
				while (!end)
				{
					m_printer->printInfo("--- Menu ---");
					m_printer->printInfo("0 - mode Bar");
					m_printer->printInfo("1 - mode Caisse");
					m_printer->printInfo("2 - mode Kve");
					m_printer->printInfo("Any other key to return to the precedent menu");
					switch (m_printer->getKeyPressed())
					{
						case '0':
							m_config->setMode(Config::BAR);
							m_printer->printInfo("Mode Bar activé");
							end = true;
							break;
						case '1':
							m_config->setMode(Config::CAISSE);
							m_printer->printInfo("Mode Caisse activé");
							end = true;
							break;
						case '2':
							m_config->setMode(Config::KVE);
							m_printer->printInfo("Mode Kve activé");
							end = true;
							break;
						default:
							end = true;
							break;
					}
				}
				m_printer->clearScreen();
				break;
			case '0':
				return true;
				break;
			case '1':
				return false;
		}
		m_printer->clearScreen();
	}
	return false;
}
