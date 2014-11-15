/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 14:14:44 by availlan          #+#    #+#             */
/*   Updated: 2014/11/15 16:26:58 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef APPLICATION_H
# define APPLICATION_H

#include "config_driver.h"
#include "python_driver.h"
#include "mode.h"

class	Application
{
	public:
		Application(bool debug, char *argv0, char *argv1 = nullptr);
		~Application();
		Application(const Application&) = delete;
		Application	&operator=(const Application&) = delete;

		bool		run();

	private:
		Mode		*create_mode();
		Printer		m_printer;
		Config		m_config;
		NfcDevice	m_nfc;
		Mysql		*m_mysql;
		Python		m_python;
		Mode		*m_mode;
};

#endif /* !APPLICATION_H */
