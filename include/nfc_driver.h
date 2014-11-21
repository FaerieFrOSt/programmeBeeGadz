/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nfc_driver.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/10/17 01:17:22 by availlan          #+#    #+#             */
/*   Updated: 2014/11/20 23:04:57 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef NFC_DRIVER_H
# define NFC_DRIVER_H

#include <nfc/nfc.h>
#include "print_driver.h"
#include "card.h"
#include "mifare.h"
#include <vector>

class   NfcDevice
{
	public:
	NfcDevice(Printer *print); //Throws an exception when something bad happened
	~NfcDevice();
	/* NfcDevice(const NfcDevice&) = delete; */
	/* NfcDevice&	operator=(const NfcDevice&) = delete; */

	bool					infiniteSelect(bool infinite);
	bool					mifareCmd(mifare_cmd key, size_t block, mifare_param *param);
	std::unique_ptr<Card>	findCard(const std::vector<uint8_t> &keys, bool infinite = false);
	bool					findCard(const uint8_t uid[8], size_t len);

	private:
	bool						m_infinite;
	nfc_device					*m_device;
	static nfc_context			*m_context;
	static int					m_instances;
	Printer						*m_print;
	bool						m_isInit;
	static const				nfc_modulation nmMifare;
};

#endif /* !NFC_DRIVER_H */
