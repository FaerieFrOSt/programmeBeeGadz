/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_driver.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/10 22:33:16 by availlan          #+#    #+#             */
/*   Updated: 2014/11/18 22:41:58 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_driver.h"
#include <algorithm>
#include "json/json.h"
#include <exception>
#include <fstream>

Config::Config(Printer *p, std::string filename) : m_print(p), m_filename(filename)
{
	Json::Value		root;
	Json::Reader	reader;
	std::ifstream	tmp(filename, std::ifstream::binary);
	if (!reader.parse(tmp, root))
	{
		m_print->printError("Error while reading config file !");
		m_print->printError(reader.getFormattedErrorMessages());
		throw std::exception();
	}
	std::string	name = root.get("mode", "bar").asString();
	if (name == "bar")
		m_mode = Config::BAR;
	else if (name == "caisse")
		m_mode = Config::CAISSE;
	else if (name == "kve")
		m_mode = Config::KVE;
	const Json::Value	mysql = root["mysql"];
	for (auto i : mysql)
		m_sql.push_back(std::array<std::string, 4>{{i.get("server", "127.0.0.1").asString(),
					i.get("database", "guinche").asString(),
					i.get("user", "root").asString(),
					i.get("password", "").asString()}});
	const Json::Value	consos = root["consos"];
	for (auto i : consos.getMemberNames())
		m_consos.push_back(std::make_pair(i, consos.get(i, 0.0f).asFloat()));
	std::string	value = root.get("ticket", "").asString();
	auto	it = std::find_if(m_consos.begin(), m_consos.end(), [value](const std::pair<std::string, float> &i)
			{
				return i.first == value;
			});
	if (it != m_consos.end())
		m_config["ticket"] = std::to_string(it - m_consos.begin());
	else
		m_config["ticket"] = std::to_string(-1);
	m_config["pianss"] = root.get("pianss", "0").asString();
}

Config::~Config()
{}
const std::pair<std::string, float>	&Config::getConso(size_t nb) const
{
	return m_consos[nb];
}

size_t	Config::getNbConso() const
{
	return m_consos.size();
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

const std::array<std::string, 4>	&Config::getSqlInfo(size_t nb)
{
	return m_sql[nb]; 
}

size_t	Config::getNbSqlInfo() const
{
	return m_sql.size();
}
