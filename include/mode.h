/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 14:48:12 by availlan          #+#    #+#             */
/*   Updated: 2014/11/11 17:30:31 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MODE_H
# define MODE_H

#include "print_driver.h"
#include "nfc_driver.h"
#include "mysql_driver.h"
#include <unordered_set>

class	Mode
{
	public:
		Mode(Printer *printer, NfcDevice *device, Mysql *sql);
		virtual ~Mode();

		virtual bool	run() = 0;

	protected:
		Printer									*m_printer;
		NfcDevice								*m_device;
		Mysql									*m_sql;
};

#endif /* !MODE_H */
