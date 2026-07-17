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
	template <typename T> void mergeInsertSort(typename T::iterator left,
		typename T::iterator right);

	template <typename Value>
	std::vector<Value> fordJohnson(std::vector<Value> arr);

	static std::vector<long> jacobsthal(size_t limit);

	double getTime(void);
	template <typename T> const char *identify(const T &);
};

#include "PmergeMe.tpp"


#endif /* PMERGEME_HPP */