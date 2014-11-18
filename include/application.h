/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 14:14:44 by availlan          #+#    #+#             */
/*   Updated: 2014/11/18 19:03:16 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef APPLICATION_H
# define APPLICATION_H

#include "config_driver.h"
#include "python_driver.h"
#include "mode.h"
#include <memory>

class	Application
{
	public:
		Application(bool debug, char *argv0, char *argv1 = nullptr);
		Application(const Application&) = delete;
		Application	&operator=(const Application&) = delete;
		~Application();

		bool		run();

	private:
		std::unique_ptr<Mode>	create_mode();
		void					clean();

		Printer					*m_printer;
		Config					*m_config;
		NfcDevice				*m_nfc;
		Mysql					*m_mysql;
		Python					m_python;
		std::unique_ptr<Mode>	m_mode;
};

#endif /* !APPLICATION_H */
