/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_driver.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/15 21:59:55 by availlan          #+#    #+#             */
/*   Updated: 2014/11/20 20:15:46 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "print_driver.h"
#include <iostream>
#include <ncurses.h>
#include <cstdio>
#include <fstream>

Printer::Printer(bool isDebug, Python *p) : m_debug(isDebug), m_python(p)
{
	initscr();
	/* cbreak(); */
	raw();
	scrollok(stdscr, true);
	idlok(stdscr, true);
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_BLACK);
	m_python->runFile("lcd.py");
}

Printer::~Printer()
{
	endwin();
}

bool	Printer::keyPressed() const
{
	nodelay(stdscr, TRUE);
	noecho();
	return getch() != ERR;
}

int	Printer::getKeyPressedNB() const
{
	nodelay(stdscr, TRUE);
	noecho();
	return getch();
}

int	Printer::getKeyPressed() const
{
	nodelay(stdscr, FALSE);
	noecho();
	return getch();
}

float	Printer::getFloat() const
{
	fseek(stdin, 0, SEEK_END);
	nodelay(stdscr, FALSE);
	echo();
	float	tmp = 0.0f;
	scanw("%f", &tmp);
	return tmp;
}

void	Printer::printLCD(std::string message, uint8_t line) const
{
	std::ostringstream	tmp;
	tmp << "lcd.lcd_display_string(\"" << message.substr(0, 20).c_str() << "\", " << line << ")";
	m_python->runScript(tmp.str());
}

void	Printer::clearLine(uint8_t line) const
{
	std::ostringstream	tmp;
	tmp << "lcd.lcd_display_string(\" \" * 20, " << line << ")";
	m_python->runScript(tmp.str());
}

void	Printer::clearScreen() const
{
	clear();
	m_python->runScript("lcd.lcd_clear()");
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
