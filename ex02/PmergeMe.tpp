/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.tpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 19:29:25 by alejhern          #+#    #+#             */
/*   Updated: 2026/07/17 19:29:28 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_TPP
# define PMERGEME_TPP

#include "PmergeMe.hpp"

class PmergeMe;

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

/*
** Implementacion recursiva de Ford-Johnson (merge-insertion sort)
** sobre un std::vector<Value> (necesitamos acceso aleatorio para que
** lower_bound sea eficiente, sea cual sea el contenedor original).
*/
template <typename Value>
std::vector<Value> PmergeMe::fordJohnson(std::vector<Value> arr)
{
	size_t n = arr.size();
	if (n <= 1)
		return (arr);

	bool	odd = (n % 2 == 1);
	Value	straggler = Value();
	if (odd)
	{
		straggler = arr.back();
		arr.pop_back();
		n--;
	}

	std::vector<std::pair<Value, Value> > pairs;
	for (size_t i = 0; i < n; i += 2)
	{
		if (arr[i] > arr[i + 1])
			pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
		else
			pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
	}

	std::vector<Value> firsts;
	for (size_t i = 0; i < pairs.size(); i++)
		firsts.push_back(pairs[i].first);
	std::vector<Value> sortedFirsts = fordJohnson<Value>(firsts);

	std::vector<std::pair<Value, Value> > sortedPairs;
	std::vector<bool> used(pairs.size(), false);
	for (size_t i = 0; i < sortedFirsts.size(); i++)
	{
		for (size_t j = 0; j < pairs.size(); j++)
		{
			if (!used[j] && pairs[j].first == sortedFirsts[i])
			{
				sortedPairs.push_back(pairs[j]);
				used[j] = true;
				break;
			}
		}
	}
	pairs = sortedPairs;

	std::vector<Value> mainChain;
	mainChain.push_back(pairs[0].second);
	for (size_t i = 0; i < pairs.size(); i++)
		mainChain.push_back(pairs[i].first);

	/*
	** Generacion CORRECTA del orden de insercion de Jacobsthal.
	** Cubre SIEMPRE todos los indices 1..pendCount exactamente una vez.
	** jac[k] = jac[k-1] + 2 * jac[k-2], jac[0]=0, jac[1]=1 -> 0,1,1,3,5,11,21,43...
	*/
	size_t pendCount = pairs.size() - 1;

	std::vector<size_t> order;
	if (pendCount >= 1)
		order.push_back(1);

	std::vector<long> jac;
	jac.push_back(0);
	jac.push_back(1);

	size_t low = 2;
	size_t k = 2;
	while (low <= pendCount)
	{
		while (jac.size() <= k)
			jac.push_back(jac[jac.size() - 1] + 2 * jac[jac.size() - 2]);

		long high = jac[k];
		if (high > static_cast<long>(pendCount))
			high = static_cast<long>(pendCount);

		for (long idx = high; idx >= static_cast<long>(low); idx--)
			order.push_back(static_cast<size_t>(idx));

		low = static_cast<size_t>(jac[k]) + 1;
		k++;
	}

	for (size_t oi = 0; oi < order.size(); oi++)
	{
		size_t	pIdx = order[oi];
		Value	value = pairs[pIdx].second;

		typename std::vector<Value>::iterator boundIt =
			std::find(mainChain.begin(), mainChain.end(), pairs[pIdx].first);
		typename std::vector<Value>::iterator pos =
			std::lower_bound(mainChain.begin(), boundIt, value);
		mainChain.insert(pos, value);
	}

	if (odd)
	{
		typename std::vector<Value>::iterator pos =
			std::lower_bound(mainChain.begin(), mainChain.end(), straggler);
		mainChain.insert(pos, straggler);
	}

	return (mainChain);
}

template <typename T> void PmergeMe::mergeInsertSort(typename T::iterator left,
	typename T::iterator right)
{
	typedef typename T::value_type value_type;

	if (std::distance(left, right) <= 1)
		return ;

	std::vector<value_type> arr(left, right);
	arr = fordJohnson<value_type>(arr);
	std::copy(arr.begin(), arr.end(), left);
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
#endif /* PMERGEME_TPP */
