/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_driver.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/10 22:33:16 by availlan          #+#    #+#             */
/*   Updated: 2014/11/17 22:49:31 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_driver.h"
#include <algorithm>
#include "json/json.h"

Config::Config(std::string filename) : m_filename(filename)
{
	m_mode = Config::BAR;
	m_config["server"] = "127.0.0.1";
	m_config["db"] = "scores";
	m_config["user"] = "root";
	m_config["password"] = "vive-moi";
	m_consos.push_back(std::make_pair("Bière", 1.0));
	m_consos.push_back(std::make_pair("Coupe de champagne", 1.5));
	auto	value = std::make_pair("B. champagne", 2);
	m_consos.push_back(value);
	auto	it = std::find_if(m_consos.begin(), m_consos.end(), [value](const std::pair<std::string, float> &i)
			{
				return i.first == value.first && i.second == value.second;
			});
	if (it != m_consos.end())
		m_config["Ticket"] = std::to_string(it - m_consos.begin());
	else
		m_config["Ticket"] = std::to_string(-1);
}

const std::pair<std::string, float>	&Config::getConso(size_t nb) const
{
	return m_consos[nb];
}

size_t	Config::getNbConso() const
{
	return m_consos.size();
}

const std::array<std::string, 4>	&Config::getSqlInfos() const
{
	return m_sqlInfos;
}

Config::Mode	Config::getMode() const
{
	return m_mode;
}

void	Config::setMode(Config::Mode mode)
{
	m_mode = mode;
}

const std::string				&Config::operator[](const std::string &name)
{
	return m_config[name];
}
