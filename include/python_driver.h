/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   python_driver.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 11:43:04 by availlan          #+#    #+#             */
/*   Updated: 2014/11/18 22:50:56 by availlan         ###   ########.fr       */
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

		void	runFile(const std::string &filename); // Keep the context
		void	runScript(const std::string &script);

	private:
		char	*m_programName;
		std::string	readFile(const std::string &filename);
};

#endif /* !PYTHON_DRIVER_H */
