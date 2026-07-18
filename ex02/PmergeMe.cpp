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
/* ------------------------ Ford-Johnson: helper steps ------------------------ */

/*
** Empareja los elementos de dos en dos, colocando siempre el mayor
** como "first" y el menor como "second". Si el numero de elementos
** es impar, el ultimo (straggler) se extrae antes y se guarda aparte.
*/
std::vector<std::pair<int, int> > PmergeMe::makePairs(std::vector<int> &arr,
	int &straggler, bool &odd)
{
	odd = (arr.size() % 2 == 1);
	if (odd)
	{
		straggler = arr.back();
		arr.pop_back();
	}

	std::vector<std::pair<int, int> > pairs;
	for (size_t i = 0; i < arr.size(); i += 2)
	{
		if (arr[i] > arr[i + 1])
			pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
		else
			pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
	}
	return (pairs);
}

/*
** Ordena recursivamente los pares segun su elemento "first"
** (llamada recursiva a fordJohnson) y reordena "pairs" para
** que coincida con ese orden.
*/
void PmergeMe::sortPairsByFirst(std::vector<std::pair<int, int> > &pairs)
{
	std::vector<int> firsts;
	for (size_t i = 0; i < pairs.size(); i++)
		firsts.push_back(pairs[i].first);

	std::vector<int> sortedFirsts = fordJohnson(firsts);

	std::vector<std::pair<int, int> > sortedPairs;
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
}

/*
** Construye la cadena principal: el "second" del primer par,
** seguido de todos los "first" ya ordenados.
*/
std::vector<int> PmergeMe::buildMainChain(const std::vector<std::pair<int, int> > &pairs)
{
	std::vector<int> mainChain;

	mainChain.push_back(pairs[0].second);
	for (size_t i = 0; i < pairs.size(); i++)
		mainChain.push_back(pairs[i].first);
	return (mainChain);
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

/*
** Inserta cada elemento pendiente en la cadena principal mediante
** busqueda binaria, respetando el orden de Jacobsthal. La busqueda
** se limita al tramo anterior a la posicion de su pareja ("first"),
** ya que se garantiza que el valor pendiente es menor que esta.
*/
void PmergeMe::insertPending(std::vector<int> &mainChain,
	const std::vector<std::pair<int, int> > &pairs,
	const std::vector<size_t> &order)
{
	for (size_t oi = 0; oi < order.size(); oi++)
	{
		size_t	pIdx = order[oi];
		int	value = pairs[pIdx].second;

		std::vector<int>::iterator boundIt =
			std::find(mainChain.begin(), mainChain.end(), pairs[pIdx].first);
		std::vector<int>::iterator pos =
			std::lower_bound(mainChain.begin(), boundIt, value);

		mainChain.insert(pos, value);
	}
}

/*
** Implementacion recursiva de Ford-Johnson (merge-insertion sort)
** sobre un std::vector<int> (necesitamos acceso aleatorio para que
** lower_bound sea eficiente, sea cual sea el contenedor original).
**
** Ahora solo orquesta los pasos: emparejar, ordenar recursivamente
** los pares, construir la cadena principal, calcular el orden de
** Jacobsthal e insertar los pendientes (y el straggler si lo hay).
*/
std::vector<int> PmergeMe::fordJohnson(std::vector<int> arr)
{
	if (arr.size() <= 1)
		return (arr);

	int	straggler = 0;
	bool	odd;

	std::vector<std::pair<int, int> > pairs = makePairs(arr, straggler, odd);
	sortPairsByFirst(pairs);

	std::vector<int>	mainChain = buildMainChain(pairs);
	std::vector<size_t>	order = jacobsthalOrder(pairs.size() - 1);

	insertPending(mainChain, pairs, order);

	if (odd)
	{
		std::vector<int>::iterator pos =
			std::lower_bound(mainChain.begin(), mainChain.end(), straggler);
		mainChain.insert(pos, straggler);
	}
	return (mainChain);
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