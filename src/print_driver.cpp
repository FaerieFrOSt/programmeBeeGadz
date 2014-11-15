/********************************/
/*      Print driver facility   */
/********************************/

#include "print_driver.h"
#include <iostream>
#include <ncurses.h>

Printer::Printer(bool isDebug) : m_debug(isDebug)
{
	initscr();
	cbreak();
	nodelay(stdscr, TRUE);
	scrollok(stdscr, true);
	idlok(stdscr, true);
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_BLACK);
}

Printer::~Printer()
{
	endwin();
}

bool	Printer::keyPressed() const
{
	return getch() != ERR;
}

void    Printer::printMessage(Printer::Type type, std::string message)
{
	std::stringstream	oss("");
	uint8_t	color = 3;
	switch (type)
	{
		case    Printer::ERROR:
			color = 1;
			oss << "ERROR : " << message << std::endl;
			break;

		case    Printer::DEBUG:
			color = 2;
			if (m_debug)
				oss << "DEBUG : " << message << std::endl;
			break;

		case    Printer::INFO:
		default:
			color = 3;
			oss << "INFO : " <<  message << std::endl;
			break;
	}
	attron(COLOR_PAIR(color));
	printw(oss.str().c_str());
	refresh();
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

bool	Printer::isDebug() const
{
	return m_debug;
}
