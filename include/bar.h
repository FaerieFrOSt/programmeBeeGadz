/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bar.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 14:55:58 by availlan          #+#    #+#             */
/*   Updated: 2014/11/11 17:00:49 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef BAR_H
# define BAR_H

#include "mode.h"

class	Bar : public Mode
{
	public:
		Bar(Printer *printer, NfcDevice *device, Mysql *sql);
		virtual ~Bar();

		virtual bool	run();
};

#endif /* !BAR_H */
