/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_driver.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/10 22:33:16 by availlan          #+#    #+#             */
/*   Updated: 2014/11/11 11:24:45 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_driver.h"

Config::Config(std::string filename) : m_filename(filename)
{

}

const std::map<std::string, size_t>	&Config::getPrices() const
{
	return m_prices;
}

const std::array<std::string, 4>	&Config::getSqlInfos() const
{
	return m_sqlInfos;
}

Config::Mode	Config::getMode() const
{
	return m_mode;
}
