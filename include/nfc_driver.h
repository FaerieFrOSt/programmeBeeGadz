/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nfc_driver.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/10/17 01:17:22 by availlan          #+#    #+#             */
/*   Updated: 2014/10/20 01:19:58 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef NFC_DRIVER_H
# define NFC_DRIVER_H

#include <nfc/nfc.h>
#include "print_driver.h"
#include "card.h"

class   NfcDevice
{
	public:
	NfcDevice(Printer *print);
	~NfcDevice();

	bool    init();
	bool    isInit() const { return m_isInit; }

	Card    *findCard();

	protected:
	nfc_device  *m_device;
	nfc_target  m_target;

	private:
	static nfc_context *m_context;
	static int  m_instances;
	Printer *m_print;
	bool    m_isInit;
};

#endif /* !NFC_DRIVER_H */
