/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bar.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 14:55:58 by availlan          #+#    #+#             */
/*   Updated: 2014/11/19 21:18:56 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef BAR_H
# define BAR_H

#include "mode.h"
#include <unordered_map>
#include <functional>
#include <utility>

class	Bar : public Mode
{
	public:
		Bar(Printer *printer, NfcDevice *device, std::function<void(std::string)> &sql, Config *config);
		virtual ~Bar();

		virtual bool	run();


		float			getPrice(const std::unordered_map<size_t, size_t>&) const;
		void			printCommand(const std::unordered_map<size_t, size_t>&) const;

	protected:
		int			getConso(Card &card);
		void		decrementTicket(Card &card);
		void		decrementCredit(Card &card, float price);
};

#endif /* !BAR_H */
