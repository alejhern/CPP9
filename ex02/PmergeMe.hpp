/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 12:43:04 by alejhern          #+#    #+#             */
/*   Updated: 2026/07/18 00:00:00 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
# define PMERGEME_HPP
# include <algorithm>
# include <climits>
# include <cstdlib>
# include <deque>
# include <iomanip>
# include <iostream>
# include <list>
# include <stdexcept>
# include <utility>
# include <vector>

class PmergeMe
{
  public:
	PmergeMe();
	PmergeMe(const PmergeMe &other);
	PmergeMe &operator=(const PmergeMe &other);
	~PmergeMe();

	template <typename T> void parse(T &container, char **argv, int argc);

	template <typename T> void sort(T &container);

	template <typename T> bool isSorted(const T &container);

	template <typename T> void printContainer(const T &container);
	class PmergeMeException : public std::runtime_error
	{
		public:
		PmergeMeException(const std::string &message);
	};

  private:
	template <typename T> void fordJohnsonSort(typename T::iterator left,
		typename T::iterator right);

	/*
	** Todo lo que sigue esta templatizado sobre T (el propio contenedor
	** de origen: std::vector<int> o std::deque<int>). El algoritmo
	** opera siempre directamente sobre T, nunca lo convierte a otro
	** tipo de contenedor distinto al de origen.
	*/
	template <typename T> static std::vector<std::pair<typename T::value_type,
		typename T::value_type> > makePairs(T &arr,
		typename T::value_type &straggler, bool &odd);

	template <typename T> static void sortPairsByFirst(
		std::vector<std::pair<typename T::value_type,
		typename T::value_type> > &pairs);

	template <typename T> static T buildMainChain(
		const std::vector<std::pair<typename T::value_type,
		typename T::value_type> > &pairs);

	static std::vector<size_t> jacobsthalOrder(size_t pendCount);

	template <typename T> static void insertPending(T &mainChain,
		const std::vector<std::pair<typename T::value_type,
		typename T::value_type> > &pairs,
		const std::vector<size_t> &order);

	template <typename T> static T fordJohnson(T arr);

	static std::vector<long> jacobsthal(size_t limit);

	double getTime(void);
	template <typename T> const char *identify(const T &);
};

#include "PmergeMe.tpp"

#endif /* PMERGEME_HPP */