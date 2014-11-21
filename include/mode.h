/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 14:48:12 by availlan          #+#    #+#             */
/*   Updated: 2014/11/19 12:56:15 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MODE_H
# define MODE_H

#include "print_driver.h"
#include "nfc_driver.h"
#include "mysql_driver.h"
#include "card.h"
#include "config_driver.h"
#include <string>
#include <functional>
#include <utility>
#include <array>

class	Mode
{
	public:
		Mode(Printer *printer, NfcDevice *device, std::function<void(std::string)> &sql, Config *config);
		virtual ~Mode();

		virtual bool	run() = 0;

		bool		isAdmin(Card &card);
		bool		isSOS(Card &card);
		bool		isDebit(Card &card);
		bool		isConso(Card &card);

		float		getCredit(Card &card);
		bool		hasTicket(Card &card);
		uint8_t		getTicket(Card &card);

		void		createDebit(Card &card);

		void		sendSOS();

		void		sendHistory(std::string command, float price);

	protected:
		std::array<std::pair<std::string, size_t>, 4>	m_screen;
		std::array<std::time_t, 4>			m_time;
		std::array<bool, 4>				m_printed;

		void		setCommand(std::string command, size_t line, size_t time = 0);
		void		print();
		bool		testCard(Card &card, std::string str);
		void		writeCard(Card &card);
		Printer								*m_printer;
		NfcDevice							*m_device;
		std::function<void(std::string)>	m_sql;
		Config								*m_config;
};

#endif /* !MODE_H */
