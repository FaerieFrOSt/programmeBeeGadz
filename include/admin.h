/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   admin.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/15 16:17:46 by availlan          #+#    #+#             */
/*   Updated: 2014/11/19 12:56:36 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef ADMIN_H
# define ADMIN_H

#include "mode.h"

class	Admin : public Mode
{
	public:
		Admin(Printer *printer, NfcDevice *device, std::function<void(std::string)> &sql, Config *config);
		virtual ~Admin();

		virtual bool	run();

	protected:
		void	createAdmin(Card &card);
		void	createConso(Card &card, uint8_t id);
		void	createSOS(Card &card);

};

#endif /* !ADMIN_H */
