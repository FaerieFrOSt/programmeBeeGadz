/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/10/27 18:47:04 by availlan          #+#    #+#             */
/*   Updated: 2014/11/01 16:56:02 by availlan         ###   ########.fr       */
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
			//Trailer block access bits
			A_W_A = 1 << 9,
			B_W_A = 1 << 8,
			A_R_C = 1 << 7,
			B_R_C = 1 << 6,
			A_W_C = 1 << 5,
			B_W_C = 1 << 4,
			A_R_B = 1 << 3,
			A_W_B = 1 << 1,
			B_W_B = 1 << 0,
			//Normal block access bits
			A_R = 1 << 7,
			B_R = 1 << 6,
			A_W = 1 << 5,
			B_W = 1 << 4,
			A_I = 1 << 3,
			B_I = 1 << 2,
			A_D = 1 << 1,
			B_D = 1 << 0,
		};

		void			setTrailer(bool trailer);
		void			setKeyA(uint8_t keyA[6]);
		void			setKeyB(uint8_t keyB[6]);
		bool			setPermissions(size_t block, Flag permissions);
		void			useKeyB(bool keyB);
		void			setAuthentificationKey(const uint8_t *key);

		bool			keyB() const;
		const uint8_t	*authentificationKey() const;
		size_t			size() const;
		bool			verifyBCC() const;

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
		const static uint16_t	m_accessTrailer[8];
		const static uint8_t	m_accessBlock[8];
};

#endif /* !SECTOR_H */
