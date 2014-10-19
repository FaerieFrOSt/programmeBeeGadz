/********************************/
/*      Print driver facility   */
/********************************/

#include "print_driver.h"
#include <iostream>

Printer::Printer(bool isDebug) : m_debug(isDebug)
{}

Printer::~Printer()
{}

void    Printer::printMessage(Printer::Type type, std::string message)
{
	switch (type)
	{
		case    Printer::ERROR:
			std::cout << "\033[30mERROR : " << message << "\033[0m" << std::endl;
			break;

		case    Printer::DEBUG:
			if (m_debug)
				std::cout << "\033[34mDEBUG : " << message << "\033[0m" << std::endl;
			break;

		case    Printer::INFO:
		default:
			std::cout << message;
			break;
	}
}

void    Printer::printDebug(std::string message)
{
	printMessage(Printer::DEBUG, message);
}

void    Printer::printError(std::string message)
{
	printMessage(Printer::ERROR, message);
}

void    Printer::printInfo(std::string message)
{
	printMessage(Printer::INFO, message);
}
