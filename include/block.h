/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/10/27 18:49:30 by availlan          #+#    #+#             */
/*   Updated: 2014/11/13 19:44:03 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef BLOCK_H
# define BLOCK_H

#include <array>

class	Block
{
	public:
		Block(uint8_t *data = nullptr);
		Block(const Block&);
		Block	&operator=(const Block&);

		uint8_t									&operator[](size_t block);

		uint8_t									*data();
		size_t									size() const;

		std::array<uint8_t, 16>::iterator		begin();
		std::array<uint8_t, 16>::iterator		end();
		std::array<uint8_t, 16>::const_iterator	cbegin();
		std::array<uint8_t, 16>::const_iterator	cend();

		enum	State { CLEAN, DIRTY, MODIFIED, };

		State									getState() const;
		void									setState(State state);

	private:
		std::array<uint8_t, 16>	m_data;
		State					m_state;
};

#endif /* !BLOCK_H */
