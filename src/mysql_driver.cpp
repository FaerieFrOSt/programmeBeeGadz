#include "mysql_driver.h"
#include <exception>

Mysql::Mysql(std::string server, std::string db, std::string user, std::string passwd) : 
	m_server(server), m_db(db), m_user(user), m_passwd(passwd), m_result(nullptr), m_nbFields(0)
{
	mysql_init(&m_mysql);
	mysql_options(&m_mysql, MYSQL_READ_DEFAULT_GROUP, "option");
	if (mysql_real_connect(&m_mysql, server.c_str(), user.c_str(), passwd.c_str(), db.c_str(), 0, 0, 0))
		mysql_close(&m_mysql);
	else
		throw std::exception();
}

Mysql::~Mysql()
{
	if (m_result)
		mysql_free_result(m_result);
	mysql_close(&m_mysql);
}

bool	Mysql::sendRequest(const std::string request)
{
	if (!mysql_real_connect(&m_mysql, m_server.c_str(), m_user.c_str(), m_passwd.c_str(), m_db.c_str(), 0, 0, 0))
		return false;
	mysql_query(&m_mysql, request.c_str());
	mysql_close(&m_mysql);
	return true;
}

bool	Mysql::sendRequestData(const std::string request)
{
	if (!mysql_real_connect(&m_mysql, m_server.c_str(), m_user.c_str(), m_passwd.c_str(), m_db.c_str(), 0, 0, 0))
		return false;
	mysql_query(&m_mysql, request.c_str());
	if (m_result)
	{
		mysql_free_result(m_result);
		m_result = nullptr;
	}
	m_result = mysql_store_result(&m_mysql);
	m_nbFields = mysql_num_fields(m_result);
	mysql_close(&m_mysql);
	return true;
}

std::vector<std::string>	Mysql::getData(size_t nbLine)
{
	MYSQL_ROW	row;
	std::vector<std::string>	tab;
	long unsigned int	*lengths;

	mysql_data_seek(m_result, nbLine);
	row = mysql_fetch_row(m_result);
	if (!row)
		return tab;
	lengths = mysql_fetch_lengths(m_result);
	for (size_t i = 0; i < m_nbFields; ++i)
		tab.push_back(std::string(row[i], lengths[i]));
	return tab;
}

std::vector<std::string>	Mysql::getNextData()
{
	static size_t	line = 0;

	++line;
	return getData(line - 1);
}
