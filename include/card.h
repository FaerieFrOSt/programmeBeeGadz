/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   card.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/10/19 23:08:09 by availlan          #+#    #+#             */
/*   Updated: 2014/10/27 16:47:53 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CARD_H
# define CARD_H

#include <nfc/nfc-types.h>
#include <nfc/nfc.h>
#include "mifare.h"
#include "print_driver.h"
#include <functional>

#define sizeBlock 16

class   Card
{
	public:
	Card(void* device, Printer *print, nfc_target target); // Throws an exception when there is an error
	~Card();
	Card(const Card&) = delete;
	Card&	operator=(const Card&) = delete;

	bool		read();
	uint8_t		**read(size_t sector); //sends back a copy of the actual data. the user should delete it if not needed. You can use the provided static function Card::deleteSector(uint8_t**)
	bool		write(size_t sector, uint8_t *data, size_t len);
	bool		write();

	static void	deleteSector(uint8_t **sector);

	void		setKeyB(bool keyB);

	bool		operator==(Card &other);
	bool		operator!=(Card &other);

	enum		Type { MifareClassic = 0x4, };

	private:
	enum		State { CLEAN, DIRTY, MODIFIED, };

	static const uint8_t    m_keys[];

	bool    readMifareClassic(size_t sector);
	bool	writeMifareClassic(size_t sector);
	bool    authenticate(size_t sector);

	bool    readData(size_t sector);
	bool    writeData(size_t sector);
	uint8_t	*getUid();

	nfc_target					m_target;
	mifare_param				m_param;
	Type						m_type;
	Printer						*m_print;
	void						*m_device;
	uint8_t						***m_data;
	State					    *m_sectorState;
	size_t						m_uidLen;
	size_t						m_nbSectors;
	size_t						m_sectorLen;
	std::function<bool(size_t)>	m_readCard;
	std::function<bool(size_t)>	m_writeCard;
	bool						m_keyB;
};

#endif /* !CARD_H */
