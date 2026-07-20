/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 12:43:10 by alejhern          #+#    #+#             */
/*   Updated: 2026/07/20 00:00:00 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <sys/time.h>

/* --------------------------- canonical form --------------------------- */

PmergeMe::PmergeMe() : _vecTime(0.0), _deqTime(0.0)
{
}

PmergeMe::PmergeMe(const PmergeMe &other)
{
	*this = other;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	if (this != &other)
	{
		_raw = other._raw;
		_vec = other._vec;
		_deq = other._deq;
		_vecTime = other._vecTime;
		_deqTime = other._deqTime;
	}
	return (*this);
}

PmergeMe::~PmergeMe()
{
}

PmergeMe::PmergeMeException::PmergeMeException(const std::string &message)
	: std::runtime_error(message)
{
}

/* -------------------------------- timing -------------------------------- */

double PmergeMe::getTime(void) const
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000.0 + tv.tv_usec);
}

/* -------------------------------- parsing -------------------------------- */

long PmergeMe::parseOne(const std::string &token)
{
	if (token.empty())
		throw PmergeMeException("empty argument");

	for (size_t i = 0; i < token.size(); i++)
	{
		if (i == 0 && (token[i] == '+' || token[i] == '-'))
			continue;
		if (!std::isdigit(static_cast<unsigned char>(token[i])))
			throw PmergeMeException("not a number: " + token);
	}

	char *end;
	errno = 0;
	long value = std::strtol(token.c_str(), &end, 10);

	if (*end != '\0')
		throw PmergeMeException("invalid number: " + token);
	if (errno == ERANGE || value < 0 || value > INT_MAX)
		throw PmergeMeException("number out of range: " + token);
	return (value);
}

void PmergeMe::parseArgs(char **argv, int argc)
{
	_raw.clear();
	for (int i = 1; i < argc; i++)
	{
		long value = parseOne(argv[i]);

		if (std::find(_raw.begin(), _raw.end(), static_cast<int>(value)) != _raw.end())
			throw PmergeMeException("duplicate number: " + std::string(argv[i]));
		_raw.push_back(static_cast<int>(value));
	}
	if (_raw.empty())
		throw PmergeMeException("no numbers given");
}

/* ============================================================ */
/*                     JACOBSTHAL (sin contenedor)              */
/* ============================================================ */

/*
** Genera el orden de insercion de Jacobsthal para los elementos
** pendientes (indices 1..pendCount). No depende de vector ni deque,
** trabaja solo con indices (size_t / long).
*/
std::vector<size_t> PmergeMe::jacobsthalOrder(size_t pendCount)
{
	std::vector<size_t> order;

	if (pendCount < 1)
		return (order);
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
	return (order);
}

/* ============================================================ */
/*                    VERSION std::vector<int>                  */
/* ============================================================ */

std::vector<std::pair<int, int> > PmergeMe::makePairsVector(
	std::vector<int> &arr, int &straggler, bool &odd)
{
	odd = (arr.size() % 2 == 1);
	if (odd)
	{
		straggler = arr.back();
		arr.pop_back();
	}

	std::vector<std::pair<int, int> > pairs;
	std::vector<int>::iterator it = arr.begin();

	while (it != arr.end())
	{
		int a = *it++;
		int b = *it++;

		if (a > b)
			pairs.push_back(std::make_pair(a, b));
		else
			pairs.push_back(std::make_pair(b, a));
	}
	return (pairs);
}

void PmergeMe::sortPairsByFirstVector(std::vector<std::pair<int, int> > &pairs)
{
	std::vector<int> firsts;
	for (size_t i = 0; i < pairs.size(); i++)
		firsts.push_back(pairs[i].first);

	std::vector<int> sortedFirsts = fordJohnsonVector(firsts);

	std::vector<std::pair<int, int> > sortedPairs;
	std::vector<bool> used(pairs.size(), false);

	for (std::vector<int>::iterator sfIt = sortedFirsts.begin();
		 sfIt != sortedFirsts.end(); ++sfIt)
	{
		for (size_t j = 0; j < pairs.size(); j++)
		{
			if (!used[j] && pairs[j].first == *sfIt)
			{
				sortedPairs.push_back(pairs[j]);
				used[j] = true;
				break;
			}
		}
	}
	pairs = sortedPairs;
}

std::vector<int> PmergeMe::buildMainChainVector(
	const std::vector<std::pair<int, int> > &pairs)
{
	std::vector<int> mainChain;

	mainChain.push_back(pairs[0].second);
	for (size_t i = 0; i < pairs.size(); i++)
		mainChain.push_back(pairs[i].first);
	return (mainChain);
}

void PmergeMe::insertPendingVector(std::vector<int> &mainChain,
								   const std::vector<std::pair<int, int> > &pairs,
								   const std::vector<size_t> &order)
{
	for (size_t oi = 0; oi < order.size(); oi++)
	{
		size_t pIdx = order[oi];
		int value = pairs[pIdx].second;

		std::vector<int>::iterator boundIt =
			std::find(mainChain.begin(), mainChain.end(), pairs[pIdx].first);
		std::vector<int>::iterator pos =
			std::lower_bound(mainChain.begin(), boundIt, value);

		mainChain.insert(pos, value);
	}
}

/*
** Ford-Johnson (merge-insertion sort) trabajando unicamente con
** std::vector<int> / std::vector<std::pair<int,int> > de principio a
** fin. No hay conversion a deque ni a ningun otro contenedor.
*/
std::vector<int> PmergeMe::fordJohnsonVector(std::vector<int> arr)
{
	if (arr.size() <= 1)
		return (arr);

	int straggler = 0;
	bool odd;

	std::vector<std::pair<int, int> > pairs = makePairsVector(arr, straggler, odd);
	sortPairsByFirstVector(pairs);

	std::vector<int> mainChain = buildMainChainVector(pairs);
	std::vector<size_t> order = jacobsthalOrder(pairs.size() - 1);

	insertPendingVector(mainChain, pairs, order);

	if (odd)
	{
		std::vector<int>::iterator pos =
			std::lower_bound(mainChain.begin(), mainChain.end(), straggler);
		mainChain.insert(pos, straggler);
	}
	return (mainChain);
}

void PmergeMe::sortVector(void)
{
	_vec = _raw;

	double start = getTime();
	_vec = fordJohnsonVector(_vec);
	double end = getTime();

	_vecTime = end - start;
}

/* ============================================================ */
/*                     VERSION std::deque<int>                  */
/* ============================================================ */

std::deque<std::pair<int, int> > PmergeMe::makePairsDeque(
	std::deque<int> &arr, int &straggler, bool &odd)
{
	odd = (arr.size() % 2 == 1);
	if (odd)
	{
		straggler = arr.back();
		arr.pop_back();
	}

	std::deque<std::pair<int, int> > pairs;
	std::deque<int>::iterator it = arr.begin();

	while (it != arr.end())
	{
		int a = *it++;
		int b = *it++;

		if (a > b)
			pairs.push_back(std::make_pair(a, b));
		else
			pairs.push_back(std::make_pair(b, a));
	}
	return (pairs);
}

void PmergeMe::sortPairsByFirstDeque(std::deque<std::pair<int, int> > &pairs)
{
	std::deque<int> firsts;
	for (size_t i = 0; i < pairs.size(); i++)
		firsts.push_back(pairs[i].first);

	std::deque<int> sortedFirsts = fordJohnsonDeque(firsts);

	std::deque<std::pair<int, int> > sortedPairs;
	std::vector<bool> used(pairs.size(), false);

	for (std::deque<int>::iterator sfIt = sortedFirsts.begin();
		 sfIt != sortedFirsts.end(); ++sfIt)
	{
		for (size_t j = 0; j < pairs.size(); j++)
		{
			if (!used[j] && pairs[j].first == *sfIt)
			{
				sortedPairs.push_back(pairs[j]);
				used[j] = true;
				break;
			}
		}
	}
	pairs = sortedPairs;
}

std::deque<int> PmergeMe::buildMainChainDeque(
	const std::deque<std::pair<int, int> > &pairs)
{
	std::deque<int> mainChain;

	mainChain.push_back(pairs[0].second);
	for (size_t i = 0; i < pairs.size(); i++)
		mainChain.push_back(pairs[i].first);
	return (mainChain);
}

void PmergeMe::insertPendingDeque(std::deque<int> &mainChain,
								  const std::deque<std::pair<int, int> > &pairs,
								  const std::vector<size_t> &order)
{
	for (size_t oi = 0; oi < order.size(); oi++)
	{
		size_t pIdx = order[oi];
		int value = pairs[pIdx].second;

		std::deque<int>::iterator boundIt =
			std::find(mainChain.begin(), mainChain.end(), pairs[pIdx].first);
		std::deque<int>::iterator pos =
			std::lower_bound(mainChain.begin(), boundIt, value);

		mainChain.insert(pos, value);
	}
}

/*
** Ford-Johnson (merge-insertion sort) trabajando unicamente con
** std::deque<int> / std::deque<std::pair<int,int> > de principio a
** fin. No hay conversion a vector ni a ningun otro contenedor.
*/
std::deque<int> PmergeMe::fordJohnsonDeque(std::deque<int> arr)
{
	if (arr.size() <= 1)
		return (arr);

	int straggler = 0;
	bool odd;

	std::deque<std::pair<int, int> > pairs = makePairsDeque(arr, straggler, odd);
	sortPairsByFirstDeque(pairs);

	std::deque<int> mainChain = buildMainChainDeque(pairs);
	std::vector<size_t> order = jacobsthalOrder(pairs.size() - 1);

	insertPendingDeque(mainChain, pairs, order);

	if (odd)
	{
		std::deque<int>::iterator pos =
			std::lower_bound(mainChain.begin(), mainChain.end(), straggler);
		mainChain.insert(pos, straggler);
	}
	return (mainChain);
}

void PmergeMe::sortDeque(void)
{
	_deq.assign(_raw.begin(), _raw.end());

	double start = getTime();
	_deq = fordJohnsonDeque(_deq);
	double end = getTime();

	_deqTime = end - start;
}

/* -------------------------------- printing -------------------------------- */

void PmergeMe::printBefore(void) const
{
	std::cout << "Before: ";
	for (size_t i = 0; i < _raw.size(); i++)
	{
		std::cout << _raw[i];
		if (i + 1 < _raw.size())
			std::cout << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::printAfter(void) const
{
	std::cout << "After: ";
	for (size_t i = 0; i < _vec.size(); i++)
	{
		std::cout << _vec[i];
		if (i + 1 < _vec.size())
			std::cout << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::printTimes(void) const
{
	std::cout << "Time to process a range of " << _raw.size()
			  << " elements with std::vector : "
			  << std::fixed << std::setprecision(5) << _vecTime << " us" << std::endl;
	std::cout << "Time to process a range of " << _raw.size()
			  << " elements with std::deque : "
			  << std::fixed << std::setprecision(5) << _deqTime << " us" << std::endl;
}
