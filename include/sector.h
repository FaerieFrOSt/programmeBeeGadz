/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/10/27 18:47:04 by availlan          #+#    #+#             */
/*   Updated: 2014/10/30 11:29:14 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SECTOR_H
# define SECTOR_H

#include <vector>
#include <array>
#include "block.h"

class	Sector
{
	public:
		Sector(bool isTrailer, size_t nbBlock, bool useKeyB = false);
		Sector(const Sector&);
		Sector	&operator=(const Sector&);

		Block			&operator[](size_t block);

		enum	State { CLEAN, DIRTY, MODIFIED, };

		enum	Flag
		{
			READ = 0x1,
			WRITE = 0x2,
			USE_KEYA = 0x4,
			USE_KEYB = 0x8,
			NONE = 0x10,
			ACCESS_KEYA = 0x20,
			ACCESS_KEYB = 0x40,
			BITS = 0x80,
		};

		void			setTrailer(bool trailer);
		void			setKeyA(uint8_t keyA[6]);
		void			setKeyB(uint8_t keyB[6]);
		void			setPermissions(Flag permissions);
		void			useKeyB(bool keyB);
		void			setAuthentificationKey(const uint8_t *key);

		bool			keyB() const;
		const uint8_t	*authentificationKey() const;
		size_t			size() const;
		bool			verifyBCC() const

		State	getState() const;
		void	setState(State state);

		std::vector<Block>::iterator		begin();
		std::vector<Block>::iterator		end();
		std::vector<Block>::const_iterator	cbegin();
		std::vector<Block>::const_iterator	cend();

	private:
		std::vector<Block>		m_data;
		std::array<uint8_t, 6>	m_key;
		State					m_state;
		bool					m_isTrailer;
		bool					m_keyB;
};

#endif /* !SECTOR_H */
