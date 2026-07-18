/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 12:43:04 by alejhern          #+#    #+#             */
/*   Updated: 2026/05/19 12:43:05 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
# define PMERGEME_HPP
# include <algorithm>
# include <climits>
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
	static inline std::vector<std::pair<int, int> > makePairs(std::vector<int> &arr,
    int &straggler, bool &odd);

	static void sortPairsByFirst(std::vector<std::pair<int, int> > &pairs);

	static std::vector<int> buildMainChain(const std::vector<std::pair<int, int> > &pairs);

	static std::vector<size_t> jacobsthalOrder(size_t pendCount);

	static void insertPending(std::vector<int> &mainChain,
		const std::vector<std::pair<int, int> > &pairs,
		const std::vector<size_t> &order);

	static std::vector<int> fordJohnson(std::vector<int> arr);

	static std::vector<long> jacobsthal(size_t limit);

	double getTime(void);
	template <typename T> const char *identify(const T &);
};

#include "PmergeMe.tpp"


#endif /* PMERGEME_HPP */