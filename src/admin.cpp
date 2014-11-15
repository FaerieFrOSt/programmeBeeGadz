#include "admin.h"

Admin::Admin(Printer *printer, NfcDevice *device, Mysql *sql, Config *config) :
	Mode(printer, device, sql, config)
{}

Admin::~Admin()
{}

bool	Admin::run()
{
	m_printer->printInfo("Mode admin");
	m_printer->printInfo("--- Menu ---");
	m_printer->printInfo("0 - quit Admin mode");
	m_printer->printInfo("1 - quit program");
	m_printer->printInfo("2 - set mode");
	bool end;
	while (true)
		switch (m_printer->getKeyPressed())
		{
			case '2':
				m_printer->clearScreen();
				m_printer->printInfo("--- Menu ---");
				m_printer->printInfo("0 - mode Bar");
				m_printer->printInfo("1 - mode Caisse");
				m_printer->printInfo("2 - mode Kve");
				m_printer->printInfo("Any other key to return to the precedent menu");
				end = false;
				while (!end)
					switch (m_printer->getKeyPressed())
					{
						case '0':
							m_config->setMode(Config::BAR);
							m_printer->printInfo("Mode Bar activé");
							end = true;
							break;
						case '1':
							m_config->setMode(Config::CAISSE);
							m_printer->printInfo("Mode Caisse activé");
							end = true;
							break;
						case '2':
							end = true;
							break;
					}
				m_printer->clearScreen();
				m_printer->printInfo("--- Menu ---");
				m_printer->printInfo("0 - quit Admin mode");
				m_printer->printInfo("1 - quit program");
				m_printer->printInfo("2 - set mode");
				break;
			case '0':
				return false;
				break;
			case '1':
				return true;
		}
	return false;
}
