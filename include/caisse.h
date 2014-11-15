/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Caisse.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/15 16:54:34 by availlan          #+#    #+#             */
/*   Updated: 2014/11/15 16:58:45 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CAISSE_H
# define CAISSE_H

#include "mode.h"

class	Caisse : public Mode
{
	public:
		Caisse(Printer *printer, NfcDevice *device, Mysql *sql, Config *config);
		virtual ~Caisse();

		virtual bool	run();
};
#endif /* !CAISSE_H */
