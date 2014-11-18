/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kve.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/17 20:08:33 by availlan          #+#    #+#             */
/*   Updated: 2014/11/18 22:05:02 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef KVE_H
# define KVE_H

#include "bar.h"

class	Kve : public Bar
{
	public:
		Kve(Printer *printer, NfcDevice *device, std::function<void(std::string&)> &sql, Config *config);
		virtual ~Kve();

		virtual bool	run();
};

#endif /* !KVE_H */
