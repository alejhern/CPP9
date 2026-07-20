/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 12:43:04 by alejhern          #+#    #+#             */
/*   Updated: 2026/07/20 00:00:00 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <algorithm>
# include <cctype>
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <deque>
# include <iomanip>
# include <iostream>
# include <stdexcept>
# include <utility>
# include <vector>

/*
** PmergeMe implementa el algoritmo merge-insertion (Ford-Johnson) dos
** veces, una vez por contenedor: una version trabaja unicamente con
** std::vector<int> (y std::vector<std::pair<int,int> > como estructura
** auxiliar de pares) y la otra unicamente con std::deque<int> (y
** std::deque<std::pair<int,int> > como auxiliar). No existe ninguna
** funcion generica ni template que unifique ambas: son dos caminos de
** codigo independientes, tal y como recomienda el subject.
*/
class PmergeMe
{
	public:
		PmergeMe();
		PmergeMe(const PmergeMe &other);
		PmergeMe &operator=(const PmergeMe &other);
		~PmergeMe();

		class PmergeMeException : public std::runtime_error
		{
			public:
				PmergeMeException(const std::string &message);
		};

		void	parseArgs(char **argv, int argc);

		void	sortVector(void);
		void	sortDeque(void);

		void	printBefore(void) const;
		void	printAfter(void) const;
		void	printTimes(void) const;

	private:
		std::vector<int>	_raw;		/* secuencia original, sin ordenar */
		std::vector<int>	_vec;		/* copia ordenada con std::vector */
		std::deque<int>	_deq;		/* copia ordenada con std::deque */

		double	_vecTime;
		double	_deqTime;

		double	getTime(void) const;

		static long	parseOne(const std::string &token);

		/* --------------------- Jacobsthal (comun, sin contenedor) --------------------- */
		static std::vector<size_t>	jacobsthalOrder(size_t pendCount);

		/* ------------------------- version std::vector<int> ------------------------- */
		static std::vector<std::pair<int, int> >	makePairsVector(
				std::vector<int> &arr, int &straggler, bool &odd);
		static void	sortPairsByFirstVector(
				std::vector<std::pair<int, int> > &pairs);
		static std::vector<int>	buildMainChainVector(
				const std::vector<std::pair<int, int> > &pairs);
		static void	insertPendingVector(std::vector<int> &mainChain,
				const std::vector<std::pair<int, int> > &pairs,
				const std::vector<size_t> &order);
		static std::vector<int>	fordJohnsonVector(std::vector<int> arr);

		/* -------------------------- version std::deque<int> -------------------------- */
		static std::deque<std::pair<int, int> >	makePairsDeque(
				std::deque<int> &arr, int &straggler, bool &odd);
		static void	sortPairsByFirstDeque(
				std::deque<std::pair<int, int> > &pairs);
		static std::deque<int>	buildMainChainDeque(
				const std::deque<std::pair<int, int> > &pairs);
		static void	insertPendingDeque(std::deque<int> &mainChain,
				const std::deque<std::pair<int, int> > &pairs,
				const std::vector<size_t> &order);
		static std::deque<int>	fordJohnsonDeque(std::deque<int> arr);
};

#endif /* PMERGEME_HPP */
