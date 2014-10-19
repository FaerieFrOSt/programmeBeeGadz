/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_driver.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/10/19 22:30:28 by availlan          #+#    #+#             */
/*   Updated: 2014/10/19 23:54:57 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PRINT_DRIVER_H
# define PRINT_DRIVER_H

#include <string>

class   Printer
{
	public:
	Printer(bool isDebug);
	~Printer();

	enum    Type {INFO, ERROR, DEBUG};
	void    printMessage(Type type, std::string message);
	void    printDebug(std::string message);
	void    printError(std::string message);
	void    printInfo(std::string message);

	private:
	bool    m_debug;
};

#endif /* !PRINT_DRIVER_H */
