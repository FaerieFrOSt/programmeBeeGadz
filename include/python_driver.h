/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   python_driver.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 11:43:04 by availlan          #+#    #+#             */
/*   Updated: 2015/01/18 11:54:34 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PYTHON_DRIVER_H
# define PYTHON_DRIVER_H

#include <string>

class	Python
{
	public:
		Python(char *programName);
		Python(const Python&) = delete;
		Python	&operator=(const Python&) = delete;
		~Python();

		int	runFile(const std::string &filename); // Keep the context
		int runScript(const std::string &script);

	private:
		char	*m_programName;
		std::string	readFile(const std::string &filename);
};

#endif /* !PYTHON_DRIVER_H */
