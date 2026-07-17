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
	double getTime(void);
	template <typename T> const char *identify(const T &);
};

template <typename T> const char *PmergeMe::identify(const T &)
{
	return ("unknown container");
}

template <> inline const char *PmergeMe::identify<std::vector<int> >(const std::vector<int> &)
{
	return ("vector");
}

template <> inline const char *PmergeMe::identify<std::deque<int> >(const std::deque<int> &)
{
	return ("deque");
}

template <> inline const char *PmergeMe::identify<std::list<int> >(const std::list<int> &)
{
	return ("list");
}

template <typename T>
void PmergeMe::parse(T &container, char **argv, int argc)
{
	char		*end;
	long		value;
	const char	*type = identify(container);

	if (type == std::string("unknown container"))
		throw PmergeMeException("Unsupported container type");

	for (int i = 1; i < argc; ++i)
	{
		value = std::strtol(argv[i], &end, 10);

		if (*end != '\0')
			throw PmergeMeException("Invalid number: " + std::string(argv[i]));

		if (value < 0 || value > INT_MAX)
			throw PmergeMeException("Number out of range: "
				+ std::string(argv[i]));

		if (std::find(container.begin(), container.end(), value) != container.end())
			throw PmergeMeException("Duplicate number: "
				+ std::string(argv[i]));

		container.push_back(static_cast<int>(value));
	}
}

template <typename T> void PmergeMe::sort(T &container)
{
	double	start;
	double	end;

	if (container.empty())
		throw PmergeMeException("Container is empty");
	if (container.size() == 1)
		throw PmergeMeException("Container has only one element");
	start = getTime();
	mergeInsertSort<T>(container.begin(), container.end());
	end = getTime();
	std::cout << "Time to sort " << identify(container) << ": " << std::fixed << std::setprecision(5) << (end
		- start) << " us" << std::endl;
}

template <typename T> void PmergeMe::mergeInsertSort(typename T::iterator left,
	typename T::iterator right)
{
	if (std::distance(left, right) <= 1)
		return ;
	typename T::iterator mid = left;
	std::advance(mid, std::distance(left, right) / 2);
	mergeInsertSort<T>(left, mid);
	mergeInsertSort<T>(mid, right);
	std::inplace_merge(left, mid, right);
}

template <typename T> void PmergeMe::printContainer(const T &container)
{
	typename T::const_iterator it = container.begin();
	while (it != container.end())
	{
		std::cout << *it;
		typename T::const_iterator next = it;
		++next;
		if (next != container.end())
			std::cout << " ";
		++it;
	}
	std::cout << std::endl;
}

template <typename T>
bool	PmergeMe::isSorted(const T &container)
{
	typename T::const_iterator it = container.begin();
	typename T::const_iterator next = it;

	if (it == container.end())
		return true;

	++next;

	while (next != container.end())
	{
		if (*it > *next)
			return false;

		++it;
		++next;
	}

	return true;
}

#endif /* PMERGEME_HPP */