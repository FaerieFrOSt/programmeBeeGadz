/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   card.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/10/19 23:08:09 by availlan          #+#    #+#             */
/*   Updated: 2014/11/20 23:03:17 by availlan         ###   ########.fr       */
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

// BE AWARE THAN KEY A IN EACH SECTOR IS SET TO 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
// SO IF YOU DONT WANT TO CHANGE IT BE SURE TO WRITE IT INSIDE BLOCK 3 OF EACH SECTOR BEFORE WRITING
// THE BLOCK 3 BACK TO THE CARD

// TODO : rewrite this class to use polymorphism and factory building.

class   Card
{
	public:
	Card(void* device, Printer *print, nfc_target target, const std::vector<uint8_t> &keys); // Throws an exception when there is an error
	Card(const Card&);
	Card&	operator=(const Card&);

	bool		readAllCard();
	bool		readSector(size_t sector);

	bool		writeSector(size_t sector);
	bool		writeAllCard();

	Sector		&operator[](size_t sector);
	bool		operator==(Card &other);
	bool		operator!=(Card &other);

	const uint8_t	*getUid() const;
	size_t			sizeUid() const;

	enum		Type { MifareClassic = 0x4, };

	private:
	bool		    readMifareClassic(size_t sector);
	bool			writeMifareClassic(size_t sector);
	bool			authenticate(size_t block);

	bool			readData(size_t block);
	bool			writeData(size_t block);
	size_t			calculateBlock(size_t sector) const;
	size_t			calculateSector(size_t block, size_t *nb = nullptr) const;


	std::vector<uint8_t>		m_keys;
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
