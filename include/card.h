/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   card.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/10/19 23:08:09 by availlan          #+#    #+#             */
/*   Updated: 2014/10/20 01:23:46 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CARD_H
# define CARD_H

#include <nfc/nfc-types.h>

class   Card
{
	public:
	Card(uint8_t uid[8], uint8_t type);
	~Card();

	bool    operator==(Card &other);

	private:
	typedef uint16_t block;
	typedef block sector[4];

	sector  m_data[16];
	uint32_t    m_uid;
	uint8_t     m_type;
};

#endif /* !CARD_H */
