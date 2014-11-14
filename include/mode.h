/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 14:48:12 by availlan          #+#    #+#             */
/*   Updated: 2014/11/14 19:14:49 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MODE_H
# define MODE_H

#include "print_driver.h"
#include "nfc_driver.h"
#include "mysql_driver.h"
#include "card.h"
#include <string>

class	Mode
{
	public:
		Mode(Printer *printer, NfcDevice *device, Mysql *sql);
		virtual ~Mode();

		virtual bool	run() = 0;

		bool		isAdmin(Card &card);
		bool		isSOS(Card &card);
		bool		isDebit(Card &card);
		bool		isConso(Card &card);

		bool		hasTicket(Card &card);
		std::string	getTicket(Card &card);
		void		decrementTicket(Card &card);
		float		getCredit(Card &card);
		void		decrementCredit(Card &card, float price);

		void		sendSOS();

		void		writeCard(Card &card);

	protected:
		Printer									*m_printer;
		NfcDevice								*m_device;
		Mysql									*m_sql;
};

#endif /* !MODE_H */
