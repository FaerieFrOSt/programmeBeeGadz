/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   card.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/10/19 23:08:09 by availlan          #+#    #+#             */
/*   Updated: 2014/10/29 17:47:18 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CARD_H
# define CARD_H

#include <nfc/nfc-types.h>
#include <nfc/nfc.h>
#include "mifare.h"
#include "print_driver.h"
#include <functional>
#include <memory>
#include "sector.h"
#include <vector>

class   Card
{
	public:
	Card(void* device, Printer *print, nfc_target target); // Throws an exception when there is an error
	Card(const Card&);
	Card&	operator=(const Card&);

	bool		readAllCard();
	bool		readSector(size_t sector);

	bool		writeSector(size_t sector);
	bool		writeAllCard();

	Sector		&operator[](size_t sector);
	bool		operator==(Card &other);
	bool		operator!=(Card &other);

	enum		Type { MifareClassic = 0x4, };

	private:
	bool		    readMifareClassic(size_t sector);
	bool			writeMifareClassic(size_t sector);
	bool			authenticate(size_t sector);

	bool			readData(size_t sector);
	bool			writeData(size_t sector);
	const uint8_t	*getUid() const;
	size_t			calculateBlock(size_t sector) const;

	nfc_target					m_target;
	mifare_param				m_param;
	Type						m_type;
	Printer						*m_print;
	void						*m_device;
	std::vector<Sector>			m_data;
	size_t						m_uidLen;
	size_t						m_nbSectors;
	std::function<bool(size_t)>	m_readCard;
	std::function<bool(size_t)>	m_writeCard;
};

#endif /* !CARD_H */
