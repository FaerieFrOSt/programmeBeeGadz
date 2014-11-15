#include "mode.h"
#include <string>

Mode::Mode(Printer *printer, NfcDevice *device, Mysql *sql) : m_printer(printer),
	m_device(device), m_sql(sql)
{}

Mode::~Mode()
{}

bool	Mode::isAdmin(Card &card)
{
	std::string	admin("ADMIN");
	std::string	tmp(reinterpret_cast<char*>(card[1][0].data()));
	return admin == tmp;
}

bool	Mode::isSOS(Card &card)
{
	(void)card;
	return false;
}

bool	Mode::isDebit(Card &card)
{
	(void)card;
	return false;
}

bool	Mode::isConso(Card &card)
{
	(void)card;
	return false;
}

bool	Mode::hasTicket(Card &card)
{
	(void)card;
	return false;
}

std::string	Mode::getTicket(Card &card)
{
	(void)card;
	return std::string("");
}

void	Mode::decrementTicket(Card &card)
{
	(void)card;
}

float	Mode::getCredit(Card &card)
{
	(void)card;
	return 0.0f;
}

void	Mode::decrementCredit(Card &card, float price)
{
	(void)card;
	(void)price;
}

void	Mode::sendSOS()
{
}

void	Mode::writeCard(Card &card)
{
	(void)card;
}
