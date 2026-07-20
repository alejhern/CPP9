/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 12:43:10 by alejhern          #+#    #+#             */
/*   Updated: 2026/07/18 00:00:00 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <sys/time.h>

double PmergeMe::getTime(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000.0 + tv.tv_usec);
}

/*
** Secuencia de Jacobsthal: 0, 1, 1, 3, 5, 11, 21, 43...
** Se usa para determinar el orden óptimo de inserción de los "menores"
** en la cadena principal, minimizando el número de comparaciones.
*/
std::vector<long> PmergeMe::jacobsthal(size_t limit)
{
	std::vector<long> j;
	j.push_back(0);
	j.push_back(1);
	while (j.back() < static_cast<long>(limit))
		j.push_back(j[j.size() - 1] + 2 * j[j.size() - 2]);
	return (j);
}

/*
** Genera el orden de insercion de Jacobsthal para los elementos
** pendientes (indices 1..pendCount). Cubre siempre todos los
** indices exactamente una vez: 1, 3, 2, 5, 4, 11, 10, 9, 8, 7, 6, ...
** jac[k] = jac[k-1] + 2 * jac[k-2], jac[0] = 0, jac[1] = 1.
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

PmergeMe::PmergeMe()
{
}

PmergeMe::PmergeMe(const PmergeMe &other)
{
	(void)other;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	(void)other;
	return (*this);
}

PmergeMe::~PmergeMe()
{
}

PmergeMe::PmergeMeException::PmergeMeException(const std::string &message) : std::runtime_error(message)
{
}