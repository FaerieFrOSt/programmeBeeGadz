#include "mode.h"
#include <string>
#include <exception>
#include <ostream>

Mode::Mode(Printer *printer, NfcDevice *device, std::function<void(std::string)> &sql, Config *config) : m_printer(printer),
	m_device(device), m_sql(sql), m_config(config)
{
}

Mode::~Mode()
{}

void	Mode::print()
{
	std::time_t	timer = std::time(nullptr);
	for (size_t i = 0; i < m_screen.size(); ++i)
	{
		if (!m_screen[i].empty())
		{
			if (m_screen[i].front().get_printed() == (std::time_t)(-1))
			{
				m_printer->clearLine(i + 1);
				m_screen[i].front().set_printed();
				m_printer->printLCD(m_screen[i].front().get_message(), i + 1);
			} else if (m_screen[i].front().get_time()
					&& timer - m_screen[i].front().get_printed() >= (int)m_screen[i].front().get_time())
			{
				m_screen[i].pop();
				m_printer->clearLine(i + 1);
			}
		}
	}	
}

void	Mode::setCommand(std::string command, size_t line, size_t time)
{
	m_screen[line].push(Line(command, time));
}

bool	Mode::testCard(Card &card, std::string str)
{
	std::string	tmp(reinterpret_cast<char*>(card[1][0].data()));
	return str == tmp;
}

bool	Mode::isAdmin(Card &card)
{
	if (testCard(card, "ADMIN"))
	{
		m_config->setMode(Config::ADMIN);
		return true;
	}
	return false;
}

bool	Mode::isSOS(Card &card)
{
	return testCard(card, "SOS");
}

bool	Mode::isDebit(Card &card)
{
	return testCard(card, "DEBIT");
}

bool	Mode::isConso(Card &card)
{
	return testCard(card, "CONSO");
}

bool	Mode::hasTicket(Card &card)
{
	if (isDebit(card) && card[1][2][0])
		return true;
	return false;
}

uint8_t	Mode::getTicket(Card &card)
{
	if (hasTicket(card))
		return card[1][2][1];
	return 0;
}

float	Mode::getCredit(Card &card)
{
	if (!isDebit(card))
		return 0.0f;
	float	credit = 0.0f;
	std::memcpy(&credit, card[1][1].data(), 4);
	return credit;
}

void	Mode::createDebit(Card &card)
{
	std::string	name("DEBIT");
	float	credit = 0.0f;
	for (size_t i = 0; i < name.size(); ++i)
		card[1][0][i] = name[i];
	card[1][0][name.size()] = 0;
	std::memcpy(&card[1][1][0], &credit, 4);
	card[1][2][0] = 0;
	card[1][2][1] = 0;
	card[1][1].setState(Block::MODIFIED);
	card[1][0].setState(Block::MODIFIED);
	card[1][2].setState(Block::MODIFIED);
	writeCard(card);
}

void	Mode::sendSOS()
{
	std::ostringstream	tmp;
	tmp << "UPDATE SOS SET sos=1 WHERE pianss=\"" << (*m_config)["pianss"] << "\";";
	m_printer->printDebug(tmp.str());
	m_sql(tmp.str());
}

void	Mode::sendHistory(std::string command, float price)
{
	std::ostringstream	tmp;
	tmp << "INSERT INTO history (pianss, command, price) VALUES (";
	tmp << "\""<< (*m_config)["pianss"] << "\", \"" << command << "\", " << price;
	tmp << ");";
	m_printer->printDebug(tmp.str());
	m_sql(tmp.str());
}

void	Mode::writeCard(Card &card)
{
	m_printer->printInfo("NE PAS ENLEVER LA CARTE, ECRITURE EN COURS");
	try
	{
		if (card.writeSector(1))
			m_printer->printInfo("OK");
		else
			m_printer->printError("NON OK");
	} catch (std::exception &e)
	{}
}

