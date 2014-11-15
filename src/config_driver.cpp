/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_driver.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/10 22:33:16 by availlan          #+#    #+#             */
/*   Updated: 2014/11/15 22:01:18 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_driver.h"

Config::Config(std::string filename) : m_filename(filename)
{
	m_mode = Config::BAR;
	m_config["server"] = "127.0.0.1";
	m_config["db"] = "scores";
	m_config["user"] = "root";
	m_config["passwd"] = "vive-moi";
	m_config["Ticket"] = "Bouteille de champagne";
	m_consos.push_back(std::make_pair("Bière", 1.0));
	m_consos.push_back(std::make_pair("Coupe de champagne", 1.5));
	m_consos.push_back(std::make_pair("Bouteille de champagne", 2));
}

const std::pair<std::string, float>	&Config::getConso(size_t nb) const
{
	return m_consos[nb];
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
