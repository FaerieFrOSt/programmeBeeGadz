/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 14:48:12 by availlan          #+#    #+#             */
/*   Updated: 2014/11/15 21:46:03 by availlan         ###   ########.fr       */
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

class	Mode
{
	public:
		Mode(Printer *printer, NfcDevice *device, Mysql *sql, Config *config);
		virtual ~Mode();

		virtual bool	run() = 0;

		bool		isAdmin(Card &card);
		bool		isSOS(Card &card);
		bool		isDebit(Card &card);
		bool		isConso(Card &card);

		int			getConso(Card &card);
		bool		hasTicket(Card &card);
		std::string	getTicket(Card &card);
		void		setTicket(Card &card, const std::string &name);
		void		decrementTicket(Card &card);
		float		getCredit(Card &card);
		void		decrementCredit(Card &card, float price);
		void		incrementCredit(Card &card, float credit);

		void		createDebit(Card &card);

		void		sendSOS();

	protected:
		bool		testCard(Card &card, std::string str);
		void		writeCard(Card &card);
		Printer		*m_printer;
		NfcDevice	*m_device;
		Mysql		*m_sql;
		Config		*m_config;
};

#endif /* !MODE_H */
