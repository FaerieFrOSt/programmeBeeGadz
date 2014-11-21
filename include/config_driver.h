/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_driver.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/10 22:25:55 by availlan          #+#    #+#             */
/*   Updated: 2014/11/20 22:53:59 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_DRIVER_H
# define CONFIG_DRIVER_H

#include <string>
#include <map>
#include <vector>
#include <utility>
#include <cstring>
#include <array>
#include "print_driver.h"

class	Config
{
	public:
		Config(Printer *p, std::string filename);
		/* Config(const Config&) = delete; */
		/* Config	&operator=(const Config&) = delete; */
		~Config();

		enum Mode {
			CAISSE,
			BAR,
			KVE,
			ADMIN,
		};

		const std::pair<std::string, float>	&getConso(size_t nb) const;
		size_t								getNbConso() const;
		Mode								getMode() const;
		void								setMode(Mode mode);

		const std::string					&operator[](const std::string &name);

		const std::array<std::string, 4>	&getSqlInfo(size_t nb);
		size_t								getNbSqlInfo() const;

		const std::vector<uint8_t>			&getKeys(bool isCaisse) const;

	private:
		Printer										*m_print;
		std::string									m_filename;
		std::vector<std::pair<std::string, float>>	m_consos;
		std::map<std::string, std::string>			m_config;
		std::vector<std::array<std::string, 4>>		m_sql;
		std::vector<uint8_t>						m_keys;
		std::vector<uint8_t>						m_key;
		Mode										m_mode;
};

#endif /* !CONFIG_DRIVER_H */
