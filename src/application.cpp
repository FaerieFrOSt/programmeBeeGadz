#include "application.h"
#include <exception>
#include "bar.h"

Application::Application(bool debug, char *argv0, char *argv1) : m_printer(debug), m_config(argv1 ? argv1 : "config.txt"),
		m_nfc(&m_printer), m_mysql(nullptr),
		m_python(argv0), m_mode(nullptr)
{
	try
	{
		m_mysql = new Mysql(m_config["server"], m_config["db"], m_config["user"], m_config["passwd"]);
	}catch (std::exception &e)
	{
		m_printer.printError("Error while connecting to mysql database");
		throw;
	}
	switch (m_config.getMode())
	{
		case Config::BAR:
			m_mode = new Bar(&m_printer, &m_nfc, m_mysql);
			break;
		default:
			m_printer.printError("Error, not a reconized mode!");
			throw std::exception();
			break;
	}
}

Application::~Application()
{
	delete m_mysql;
	delete m_mode;
}

bool	Application::run()
{
	bool	end = false;
	while (!end)
	{
		try
		{
			bool	admin = m_mode->run();
			if (admin)
				m_printer.printInfo("mode admin");
			else
				end = true;
		}catch (std::exception &e)
		{
		}
	}
	return true;
}
