#include "mode.h"
#include <string>

Mode::Mode(Printer *printer, NfcDevice *device, Mysql *sql, Config *config) : m_printer(printer),
	m_device(device), m_sql(sql), m_config(config)
{}

Mode::~Mode()
{}

bool	Mode::testCard(Card &card, std::string str)
{
	std::string	tmp(reinterpret_cast<char*>(card[1][0].data()));
	return str == tmp;
}

bool	Mode::isAdmin(Card &card)
{
	return testCard(card, "ADMIN");
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

int	Mode::getConso(Card &card)
{
	if (!isConso(card))
		return -1;
	return (int)card[1][1][0];
}

bool	Mode::hasTicket(Card &card)
{
	if (isDebit(card) && card[1][2][0])
		return true;
	return false;
}

std::string	Mode::getTicket(Card &card)
{
	if (hasTicket(card))
		return std::string(reinterpret_cast<char*>(card[1][2].data() + 1));
	return std::string("");
}

void	Mode::decrementTicket(Card &card)
{
	if (card[1][2][0])
		--card[1][2][0];
	writeCard(card);
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
	writeCard(card);
}

void	Mode::setTicket(Card &card, const std::string &name)
{
	card[1][2][0] = 1;
	for (size_t	i = 0; i < name.size(); ++i)
		card[1][2][i + 1] = name[i];
	card[1][2][name.size() + 1] = 0;
	writeCard(card);
}

void	Mode::decrementCredit(Card &card, float price)
{
	float	credit = getCredit(card);
	if (!credit)
		return;
	credit -= price;
	std::memcpy(&card[1][1][0], &credit, 4);
	writeCard(card);
}

void	Mode::incrementCredit(Card &card, float credit)
{
	if (!credit)
		return;
	credit += getCredit(card);
	std::memcpy(&card[1][1][0], &credit, 4);
	writeCard(card);
}

void	Mode::sendSOS()
{
}

void	Mode::writeCard(Card &card)
{
	m_printer->printInfo("NE PAS ENLEVER LA CARTE, ECRITURE EN COURS");
	if (card.writeSector(1))
		m_printer->printInfo("OK");
	else
		m_printer->printError("NON OK");
}

