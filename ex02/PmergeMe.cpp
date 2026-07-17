/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 12:43:10 by alejhern          #+#    #+#             */
/*   Updated: 2026/05/19 12:43:13 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <sys/time.h>

double PmergeMe::getTime(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec + tv.tv_usec / 1000000.0);
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