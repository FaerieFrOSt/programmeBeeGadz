/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nfc_driver.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/10/17 01:17:22 by availlan          #+#    #+#             */
/*   Updated: 2014/10/21 00:48:59 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef NFC_DRIVER_H
# define NFC_DRIVER_H

#include <nfc/nfc.h>
#include "print_driver.h"
#include "card.h"
#include "mifare.h"
#include <memory>

class   NfcDevice
{
	public:
	NfcDevice(Printer *print);
	~NfcDevice();

	bool    init();
	bool    isInit() const { return m_isInit; }

	bool    infiniteSelect(bool infinite);
	bool    mifareCmd(mifare_cmd key, size_t sector, mifare_param *param);
	bool    findCard();
	bool    findCard(uint8_t uid[8], size_t len);
	bool    readCard();

	void    deleteCard();

	protected:
	nfc_device  *m_device;
	nfc_target  m_target;
	std::unique_ptr<Card>   m_card;

	private:
	static nfc_context *m_context;
	static int  m_instances;
	Printer *m_print;
	bool    m_isInit;
	bool    m_isToDelete;
};

#endif /* !NFC_DRIVER_H */
