/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bar.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/15 15:44:24 by availlan          #+#    #+#             */
/*   Updated: 2014/11/17 22:31:29 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bar.h"
#include <exception>
#include <ctime>
#include <unordered_map>
#include <functional>

Bar::Bar(Printer *printer, NfcDevice *device, Mysql *sql, Config *config) : Mode(printer, device, sql, config)
{}

Bar::~Bar()
{}

void	Bar::decrementCredit(Card &card, float price)
{
	float	credit = getCredit(card);
	if (!credit)
		return;
	credit -= price;
	std::memcpy(&card[1][1][0], &credit, 4);
	card[1][1].setState(Block::MODIFIED);
	writeCard(card);
}

void	Bar::decrementTicket(Card &card)
{
	if (card[1][2][0])
		--card[1][2][0];
	card[1][2].setState(Block::MODIFIED);
	writeCard(card);
}

int	Bar::getConso(Card &card)
{
	if (!isConso(card))
		return -1;
	return (int)card[1][1][0];
}

bool	Bar::run()
{
	std::unordered_map<std::pair<std::string, float>, size_t, Bar::pair_hash, Bar::pair_cmp>	conso;
	std::time_t	time = std::time(nullptr);
	m_printer->printInfo("Mode bar is running");
	m_printer->printInfo("Posez une carte CONSO.");
	std::unique_ptr<Card>	card(nullptr);
	bool					read = false;
	while (true)
	{
		try
		{
			std::time_t	timer = std::time(nullptr);
			if (m_printer->getKeyPressedNB() == '-')
			{
				conso.clear();
				m_printer->printInfo("Commande annulée.");
				m_printer->printInfo("Posez une carte CONSO.");
				time = timer;
			}
			if (!conso.empty() && timer - time >= 10)
			{
				conso.clear();
				m_printer->printInfo("Commande annulée.");
				m_printer->printInfo("Posez une carte CONSO.");
				time = timer;
			}
			if (!card)
			{
				read = false;
				card = m_device->findCard(false);
			}
			else if (!m_device->findCard(card->getUid(), card->sizeUid()))
			{
				if (!isConso(*card))
				{
					if (!conso.empty())
						m_printer->printInfo("Command reset.");
					conso.clear();
					time = timer;
				}
				card = m_device->findCard(false);
				read = false;
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
					if (!conso.empty())
						m_printer->printInfo("Command reset.");
					conso.clear();
					time = timer;
					m_printer->printInfo("Posez une carte CONSO.");
					continue;
				}
				read = true;
				m_printer->printInfo("Lecture terminée");
			}
			if (isAdmin(*card))
				return true;
			else if (isSOS(*card))
				sendSOS();
			else if (isDebit(*card) && conso.empty())
			{
				m_printer->printDebug("Argent sur la carte : " + Printer::valueToString<float>(getCredit(*card)));
				m_printer->printInfo("Posez une carte CONSO.");
			}
			else if (isDebit(*card) && !conso.empty())
			{
				if (hasTicket(*card) && conso.count(m_config->getConso(getTicket(*card))))
				{
					m_printer->printDebug(m_config->getConso(getTicket(*card)).first);
					--conso[m_config->getConso(getTicket(*card))];
					if (!conso[m_config->getConso(getTicket(*card))])
						conso.erase(m_config->getConso(getTicket(*card)));
					m_printer->printInfo("Ticket " + m_config->getConso(getTicket(*card)).first + " utilisé.");
					decrementTicket(*card);
				}
				if (getCredit(*card) >= getPrice(conso))
				{
					m_printer->printInfo("OK");
					decrementCredit(*card, getPrice(conso));
				}
				else
					m_printer->printInfo("Pas assez d'argent sur la carte !");
				conso.clear();
				time = timer;
				m_printer->printInfo("Posez une carte CONSO.");
			}
			else if (isConso(*card))
			{
				++conso[m_config->getConso(getConso(*card))];
				printCommand(conso);
				time = timer;
				m_printer->printInfo("Posez une carte DEBIT.");
			}
			else
			{
				m_printer->printError("Carte corrompue !");
				m_printer->printInfo("Posez une carte CONSO.");
			}
		} catch (std::exception &e)
		{}
	}
}

float	Bar::getPrice(std::unordered_map<std::pair<std::string, float>, size_t, Bar::pair_hash, Bar::pair_cmp> &a) const
{
	float	price = 0.0f;
	for (auto i : a)
		price += i.second;
	return price;
}

void	Bar::printCommand(std::unordered_map<std::pair<std::string, float>, size_t, Bar::pair_hash, Bar::pair_cmp> &a) const
{
	m_printer->printInfo("Commande : ");
	for (auto i : a)
		m_printer->printInfo(i.first.first + " x" + Printer::valueToString<size_t>(i.second));
}
