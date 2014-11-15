/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bar.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/15 15:44:24 by availlan          #+#    #+#             */
/*   Updated: 2014/11/15 16:14:16 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bar.h"
#include <exception>
#include <memory>
#include <ctime>

Bar::Bar(Printer *printer, NfcDevice *device, Mysql *sql, Config *config) : Mode(printer, device, sql, config)
{}

Bar::~Bar()
{}

bool	Bar::run()
{
	std::pair<std::string, float>	conso("", 0.0f);
	std::time_t	time = std::time(nullptr);
	m_printer->printInfo("Mode bar is running");
	m_printer->printInfo("Posez une carte.");
	std::unique_ptr<Card>	card(nullptr);
	bool					read = false;
	while (true)
	{
		try
		{
			std::time_t	timer = std::time(nullptr);
			if (conso.first != "" && timer - time >= 10)
			{
				conso.first = "";
				conso.second = 0.0f;
				m_printer->printInfo("Time ellapsed, Command reset.");
				time = timer;
			}
			if (m_printer->keyPressed())
				return false;
			if (!card)
			{
				read = false;
				card = m_device->findCard(false);
			}
			else if (!m_device->findCard(card->getUid(), card->sizeUid()))
			{
				if (!isConso(*card))
				{
					if (conso.first != "")
						m_printer->printInfo("Command reset.");
					conso.first = "";
					conso.second = 0.0f;
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
				card->readSector(1);
				read = true;
				m_printer->printInfo("Lecture terminée");
			}
			if (isAdmin(*card))
				return true;
			else if (isSOS(*card))
				sendSOS();
			else if (isDebit(*card) && conso.first == "")
			{
				m_printer->printDebug("Argent sur la carte : " + Printer::valueToString<float>(getCredit(*card)));
				m_printer->printInfo("Entrez d'abord la commande");
			}
			else if (isDebit(*card) && conso.first != "")
			{
				if (hasTicket(*card) && getTicket(*card) == conso.first)
				{
					m_printer->printInfo("Ticket " + conso.first + " utilisé.");
					decrementTicket(*card);
				}
				else if (getCredit(*card) >= conso.second)
				{
					m_printer->printInfo("OK");
					decrementCredit(*card, conso.second);
				}
				else
					m_printer->printInfo("Pas assez d'argent sur la carte !");
				conso.first = "";
				conso.second = 0.0f;
				time = timer;
			}
			else if (isConso(*card))
			{
				conso = m_config->getConso(getConso(*card));
				m_printer->printInfo("Commande : " + conso.first);
				time = timer;
			}
			else
				m_printer->printError("Carte corrompue !");
		} catch (std::exception &e)
		{
			m_printer->printError("Carte corrompue !");
		}
	}
}
