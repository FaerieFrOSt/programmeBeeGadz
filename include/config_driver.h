/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_driver.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/10 22:25:55 by availlan          #+#    #+#             */
/*   Updated: 2014/11/17 20:29:05 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_DRIVER_H
# define CONFIG_DRIVER_H

#include <string>
#include <map>
#include <array>
#include <vector>
#include <utility>
#include <cstring>

class	Config
{
	public:
		Config(std::string filename);
		Config(const Config&) = delete;
		Config	&operator=(const Config&) = delete;

		enum Mode {
			CAISSE,
			BAR,
			KVE,
		};

		const std::pair<std::string, float>	&getConso(size_t nb) const;
		size_t								getNbConso() const;
		const std::array<std::string, 4>	&getSqlInfos() const;
		Mode								getMode() const;
		void								setMode(Mode mode);

		const std::string					&operator[](const std::string &name);

	private:
		std::string									m_filename;
		std::vector<std::pair<std::string, float>>	m_consos;
		std::map<std::string, std::string>			m_config;
		std::array<std::string, 4>					m_sqlInfos;
		Mode										m_mode;
};

#endif /* !CONFIG_DRIVER_H */
