/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   card.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/10/19 23:08:09 by availlan          #+#    #+#             */
/*   Updated: 2014/10/21 21:55:17 by availlan         ###   ########.fr       */
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

	bool    read(size_t sector = 666);
	bool    write(size_t sector, uint8_t *data, size_t len);
	bool    write();

	bool    operator==(Card &other);
	bool    operator!=(Card &other);

	enum    Type { MifareClassic = 0x4, };
	enum    State { CLEAN, DIRTY, MODIFIED, };

	private:
	static const uint8_t    keys[];

	bool    loadMifare(size_t sector);
	bool    readData(size_t sector);
	bool    writeData(size_t sector);
	bool    authenticate(size_t sector);

	uint32_t    m_uid;
	uint8_t     m_type;
	Printer     *m_print;
	void        *m_device;
	uint8_t     m_data[16][4][16];
	State       m_sectorState[16];
};

#endif /* !CARD_H */
