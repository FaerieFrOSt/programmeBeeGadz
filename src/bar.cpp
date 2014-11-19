/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bar.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/15 15:44:24 by availlan          #+#    #+#             */
/*   Updated: 2014/11/19 23:08:08 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bar.h"
#include <exception>
#include <ctime>
#include <unordered_map>
#include <functional>

Bar::Bar(Printer *printer, NfcDevice *device, std::function<void(std::string)> &sql, Config *config) : Mode(printer, device, sql, config)
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
	std::unordered_map<size_t, size_t>	conso;
	std::time_t	time = std::time(nullptr);
	std::time_t	timePrint = std::time(nullptr);
	m_printer->printInfo("Mode bar is running");
	m_printer->printInfo("Posez une carte CONSO.");
	m_printer->printLCD("Attente d'une CONSO", 0);
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
				m_printer->clearLine(1);
				m_printer->printLCD("Commande annulée", 1);
				timePrint = timer;
				m_printer->printInfo("Posez une carte CONSO.");
				time = timer;
			}
			if (!conso.empty() && timer - time >= 10)
			{
				conso.clear();
				m_printer->printInfo("Commande annulée.");
				m_printer->clearLine(1);
				m_printer->printLCD("Commande annulée", 1);
				timePrint = timer;
				m_printer->printInfo("Posez une carte CONSO.");
				time = timer;
			}
			if (timer - timePrint >= 5)
				m_printer->clearLine(1);
			if (!card)
			{
				read = false;
				card = m_device->findCard(false);
			}
			else if (!m_device->findCard(card->getUid(), card->sizeUid()))
			{
				if (!isConso(*card) && !conso.empty())
				{
					m_printer->printInfo("Commande annulée.");
					m_printer->clearLine(1);
					m_printer->printLCD("Commande annulée", 1);
					timePrint = timer;
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
				m_printer->printLCD("  LECTURE EN COURS  ", 2);
				m_printer->printLCD("   NE PAS RETIRER   ", 3);
				try
				{
					if (!card->readSector(1))
						continue;
				} catch (std::exception &e)
				{
					if (!conso.empty())
					{
						m_printer->printInfo("Commande annulée.");
						m_printer->clearLine(1);
						m_printer->printLCD("Commande annulée", 1);
						timePrint = timer;
						conso.clear();
						time = timer;
					}
					m_printer->printInfo("Posez une carte CONSO.");
					continue;
				}
				read = true;
				m_printer->printInfo("Lecture terminée");
				m_printer->printLCD("  LECTURE TERMINEE  ", 2);
				m_printer->clearLine(3);
			}
			if (isAdmin(*card))
				return true;
			else if (isSOS(*card))
			{
				sendSOS();
				m_printer->printInfo("SOS envoyé");
				m_printer->printLCD("     SOS ENVOYE     ", 3);
			}
			else if (isDebit(*card) && conso.empty())
			{
				m_printer->printDebug("Argent sur la carte : " + Printer::valueToString<float>(getCredit(*card)));
				m_printer->printInfo("Posez une carte CONSO.");
			}
			else if (isDebit(*card) && !conso.empty())
			{
				if (hasTicket(*card) && conso.count(getTicket(*card)))
				{
					m_printer->printDebug(m_config->getConso(getTicket(*card)).first);
					--conso[getTicket(*card)];
					if (!conso[getTicket(*card)])
						conso.erase(getTicket(*card));
					m_printer->printInfo("Ticket " + m_config->getConso(getTicket(*card)).first + " utilisé.");
					sendHistory("ticket " + m_config->getConso(getTicket(*card)).first, m_config->getConso(getTicket(*card)).second);
					decrementTicket(*card);
				}
				if (getCredit(*card) >= getPrice(conso))
				{
					m_printer->printInfo("OK");
					decrementCredit(*card, getPrice(conso));
					for (auto i : conso)
						sendHistory(m_config->getConso(i.first).first + " x" + std::to_string(i.second), m_config->getConso(i.first).second * i.second);
				}
				else
					m_printer->printInfo("Pas assez d'argent sur la carte !");
				conso.clear();
				time = timer;
				m_printer->printInfo("Posez une carte CONSO.");
			}
			else if (isConso(*card))
			{
				++conso[getConso(*card)];
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
	return false;
}

float	Bar::getPrice(const std::unordered_map<size_t, size_t> &a) const
{
	float	price = 0.0f;
	for (auto i : a)
		price += m_config->getConso(i.first).second * i.second;
	return price;
}

void	Bar::printCommand(const std::unordered_map<size_t, size_t> &a) const
{
	m_printer->printInfo("Commande : ");
	for (auto i : a)
		m_printer->printInfo(m_config->getConso(i.first).first + " x" + Printer::valueToString<size_t>(i.second));
}
