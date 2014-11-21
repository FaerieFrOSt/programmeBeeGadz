/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Caisse.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/15 16:54:34 by availlan          #+#    #+#             */
/*   Updated: 2014/11/20 18:26:18 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CAISSE_H
# define CAISSE_H

#include "mode.h"

class	Caisse : public Mode
{
	public:
		Caisse(Printer *printer, NfcDevice *device, std::function<void(std::string)> &sql, Config *config);
		virtual ~Caisse();

		virtual bool	run();
	protected:
		void		incrementCredit(Card &card, float credit);
};
#endif /* !CAISSE_H */
