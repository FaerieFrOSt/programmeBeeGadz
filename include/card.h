/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   card.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/10/19 23:08:09 by availlan          #+#    #+#             */
/*   Updated: 2014/10/21 01:28:47 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CARD_H
# define CARD_H

#include <nfc/nfc-types.h>
#include "print_driver.h"

class   Card
{
	public:
	Card(void* device, Printer *print, uint8_t uid[8], uint8_t type);
	~Card();

	bool    read();
	bool    write() { return false; }

	bool    operator==(Card &other);
	bool    operator!=(Card &other);

	enum { MifareClassic = 0x4, };

	private:
	static const uint8_t    keys[];

	bool    loadMifare();
	bool    readData(size_t sector);
	bool    authenticate(size_t sector);

	uint32_t    m_uid;
	uint8_t     m_type;
	Printer     *m_print;
	void        *m_device;
	uint8_t     m_data[16][4][16];
};

#endif /* !CARD_H */
