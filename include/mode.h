/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 14:48:12 by availlan          #+#    #+#             */
/*   Updated: 2015/01/18 14:26:55 by availlan         ###   ########.fr       */
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
#include <queue>

class	Line
{
	public:
		Line(std::string message, size_t time) : m_message(message), m_time(time), m_printed(-1)
		{}

		size_t		get_time() const { return m_time; }
		std::string	get_message() const { return m_message; }

		void		set_printed() { m_printed = std::time(nullptr); }
		std::time_t	get_printed() const { return m_printed; }

	private:
		std::string	m_message;
		size_t		m_time;
		std::time_t	m_printed;
};

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
		std::array<std::queue<Line>, 4>	m_screen;

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
