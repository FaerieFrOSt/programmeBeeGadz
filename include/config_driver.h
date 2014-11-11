/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_driver.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/10 22:25:55 by availlan          #+#    #+#             */
/*   Updated: 2014/11/10 22:35:58 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_DRIVER_H
# define CONFIG_DRIVER_H

#include <string>
#include <map>
#include <array>

class	Config
{
	public:
		Config(std::string filename);
		Config(const Config&) = delete;
		Config	&operator=(const Config&) = delete;

		enum Mode {
			CAISSE,
			BAR,
		};

		const std::map<std::string, size_t>	&getPrices() const;
		const std::array<std::string, 4>	&getSqlInfos() const;
		Mode								getMode() const;

	private:
		std::string						m_filename;
		std::map<std::string, size_t>	m_prices;
		std::array<std::string, 4>		m_sqlInfos;
		Mode							m_mode;
};

#endif /* !CONFIG_DRIVER_H */
