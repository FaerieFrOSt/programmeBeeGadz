/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   admin.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/15 16:17:46 by availlan          #+#    #+#             */
/*   Updated: 2014/11/15 16:18:51 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef ADMIN_H
# define ADMIN_H

#include "mode.h"

class	Admin : public Mode
{
	public:
		Admin(Printer *printer, NfcDevice *device, Mysql *sql, Config *config);
		virtual ~Admin();

		virtual bool	run();
};

#endif /* !ADMIN_H */
