/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bar.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 14:55:58 by availlan          #+#    #+#             */
/*   Updated: 2014/11/18 22:03:47 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef BAR_H
# define BAR_H

#include "mode.h"
#include <unordered_map>
#include <functional>
#include <utility>

class	Bar : public Mode
{
	public:
		Bar(Printer *printer, NfcDevice *device, std::function<void(std::string&)> &sql, Config *config);
		virtual ~Bar();

		virtual bool	run();


		template <class T>
		static inline void	hash_combine(size_t &seed, const T &v)
		{
			std::hash<T>	hasher;
			seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		typedef struct
		{
			inline size_t	operator()(const std::pair<std::string, float> &v) const
			{
				size_t	seed = 0;
				hash_combine(seed, v.first);
				hash_combine(seed, v.second);
				return seed;
			}
		} pair_hash;

		typedef struct
		{
			inline bool	operator()(const std::pair<std::string, float> &a, const std::pair<std::string, float> &b) const
			{
				return a.first == b.first;
			}
		} pair_cmp;

		float			getPrice(std::unordered_map<std::pair<std::string, float>, size_t, pair_hash, pair_cmp>&) const;
		void			printCommand(std::unordered_map<std::pair<std::string, float>, size_t, pair_hash, pair_cmp>&) const;

	protected:
		int			getConso(Card &card);
		void		decrementTicket(Card &card);
		void		decrementCredit(Card &card, float price);
};

#endif /* !BAR_H */
