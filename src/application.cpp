#include "application.h"
#include <exception>
#include "bar.h"
#include "admin.h"
#include "caisse.h"
#include "kve.h"

Application::Application(bool debug, char *argv0, char *argv1) : m_printer(nullptr), m_config(nullptr),
		m_nfc(nullptr), m_mysql(nullptr),
		m_python(argv0), m_mode(nullptr)
{
	m_printer = new Printer(debug);
	try
	{
		m_config = new Config(m_printer, argv1 ? argv1 : "config.txt");
	} catch(std::exception &e)
	{
		while (!m_printer->keyPressed());
		clean();
		throw;
	}
	for (size_t i = 0; i < m_config->getNbSqlInfo(); ++i)
	{
		try
		{
			m_mysql = new Mysql(m_config->getSqlInfo(i));
		} catch (std::exception &e)
		{
			m_printer->printError("Error while connecting to mysql database with config " + std::to_string(i));
		}
	}
	if (!m_mysql)
	{
		m_printer->printError("Error while connecting to database!");
		while (!m_printer->keyPressed());
		clean();
		throw std::exception();
	}
	try
	{
		m_nfc = new NfcDevice(m_printer);
	} catch (std::exception &e)
	{
		while (!m_printer->keyPressed());
		clean();
		throw;
	}
	m_mode = create_mode();
}

void	Application::clean()
{
	delete m_printer;
	delete m_config;
	delete m_mysql;
	delete m_nfc;
}

Application::~Application()
{
	clean();
}

std::unique_ptr<Mode>	Application::create_mode()
{
	switch (m_config->getMode())
	{
		case Config::BAR:
			return std::unique_ptr<Mode>(new Bar(m_printer, m_nfc, m_mysql, m_config));
		case Config::CAISSE:
			return std::unique_ptr<Mode>(new Caisse(m_printer, m_nfc, m_mysql, m_config));
		case Config::KVE:
			return std::unique_ptr<Mode>(new Kve(m_printer, m_nfc, m_mysql, m_config));
		case Config::ADMIN:
			return std::unique_ptr<Mode>(new Admin(m_printer, m_nfc, m_mysql, m_config));
		default:
			m_printer->printError("Error, mode not reconized");
			while (!m_printer->keyPressed());
			clean();
			throw std::exception();
	}
	return nullptr;
}

bool	Application::run()
{
	bool	end = false;
	while (!end)
	{
		try
		{
			if (!m_mode->run())
				end = true;
			else
			{
				m_mode = create_mode();
				m_printer->clearScreen();
			}
		}catch (std::exception &e)
		{
		}
	}
	return true;
}
