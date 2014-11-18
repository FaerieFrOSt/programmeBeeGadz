/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 14:48:12 by availlan          #+#    #+#             */
/*   Updated: 2014/11/18 22:01:34 by availlan         ###   ########.fr       */
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

class	Mode
{
	public:
		Mode(Printer *printer, NfcDevice *device, std::function<void(std::string&)> &sql, Config *config);
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

	protected:
		bool		testCard(Card &card, std::string str);
		void		writeCard(Card &card);
		Printer								*m_printer;
		NfcDevice							*m_device;
		std::function<void(std::string&)>	m_sql;
		Config								*m_config;
};

#endif /* !MODE_H */
