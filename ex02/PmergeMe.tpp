/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.tpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                #+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 19:29:25 by alejhern          #+#    #+#             */
/*   Updated: 2026/07/18 00:00:00 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_TPP
# define PMERGEME_TPP

#include "PmergeMe.hpp"

class PmergeMe;

/* ------------------------- container identification ------------------------- */

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

/* --------------------------------- parsing --------------------------------- */

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
			throw PmergeMeException("Number out of range: " + std::string(argv[i]));
		if (std::find(container.begin(), container.end(), value) != container.end())
			throw PmergeMeException("Duplicate number: " + std::string(argv[i]));

		container.push_back(static_cast<int>(value));
	}
}

/* ---------------------------------- sort ---------------------------------- */

template <typename T> void PmergeMe::sort(T &container)
{
	double	start;
	double	end;

	if (container.empty())
		throw PmergeMeException("Container is empty");
	if (container.size() == 1)
		throw PmergeMeException("Container has only one element");

	start = getTime();
	fordJohnsonSort<T>(container.begin(), container.end());
	end = getTime();

	std::cout << "Time to sort " << identify(container) << ": "
		<< std::fixed << std::setprecision(5) << (end - start) << " us" << std::endl;
}

/*
** Punto de entrada del algoritmo sobre el contenedor de origen.
** "arr" se construye con el MISMO tipo T que el contenedor original
** (vector -> vector, deque -> deque): no hay conversion a otro tipo
** de contenedor en ningun punto del proceso. fordJohnson<T> trabaja
** siempre sobre T de principio a fin, y el resultado se escribe de
** vuelta en el rango [left, right) del contenedor de origen.
*/
template <typename T> void PmergeMe::fordJohnsonSort(typename T::iterator left,
	typename T::iterator right)
{
	if (std::distance(left, right) <= 1)
		return ;

	T arr(left, right);
	arr = fordJohnson<T>(arr);
	std::copy(arr.begin(), arr.end(), left);
}

/* ------------------------ Ford-Johnson: helper steps ------------------------ */

/*
** Empareja los elementos de dos en dos, colocando siempre el mayor
** como "first" y el menor como "second". Si el numero de elementos
** es impar, el ultimo (straggler) se extrae antes y se guarda aparte.
** Trabaja directamente sobre T (vector o deque), sin convertirlo.
*/
template <typename T>
std::vector<std::pair<typename T::value_type, typename T::value_type> >
PmergeMe::makePairs(T &arr, typename T::value_type &straggler, bool &odd)
{
	typedef typename T::value_type value_type;

	odd = (arr.size() % 2 == 1);
	if (odd)
	{
		straggler = arr.back();
		arr.pop_back();
	}

	std::vector<std::pair<value_type, value_type> > pairs;
	typename T::iterator it = arr.begin();
	while (it != arr.end())
	{
		value_type a = *it++;
		value_type b = *it++;

		if (a > b)
			pairs.push_back(std::make_pair(a, b));
		else
			pairs.push_back(std::make_pair(b, a));
	}
	return (pairs);
}

/*
** Ordena recursivamente los pares segun su elemento "first"
** (llamada recursiva a fordJohnson<T>) y reordena "pairs" para
** que coincida con ese orden. Los "firsts" se recogen en un
** contenedor del mismo tipo T que el origen.
*/
template <typename T>
void PmergeMe::sortPairsByFirst(
	std::vector<std::pair<typename T::value_type, typename T::value_type> > &pairs)
{
	typedef typename T::value_type value_type;

	T firsts;
	for (size_t i = 0; i < pairs.size(); i++)
		firsts.push_back(pairs[i].first);

	T sortedFirsts = fordJohnson<T>(firsts);

	std::vector<std::pair<value_type, value_type> > sortedPairs;
	std::vector<bool> used(pairs.size(), false);

	typename T::iterator sfIt = sortedFirsts.begin();
	for (; sfIt != sortedFirsts.end(); ++sfIt)
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

/*
** Construye la cadena principal (del mismo tipo T que el origen):
** el "second" del primer par, seguido de todos los "first" ya
** ordenados.
*/
template <typename T>
T PmergeMe::buildMainChain(
	const std::vector<std::pair<typename T::value_type, typename T::value_type> > &pairs)
{
	T mainChain;

	mainChain.push_back(pairs[0].second);
	for (size_t i = 0; i < pairs.size(); i++)
		mainChain.push_back(pairs[i].first);
	return (mainChain);
}

/*
** Inserta cada elemento pendiente en la cadena principal (T) mediante
** busqueda binaria, respetando el orden de Jacobsthal. mainChain sigue
** siendo del mismo tipo T que el contenedor de origen en todo momento.
*/
template <typename T>
void PmergeMe::insertPending(T &mainChain,
	const std::vector<std::pair<typename T::value_type, typename T::value_type> > &pairs,
	const std::vector<size_t> &order)
{
	for (size_t oi = 0; oi < order.size(); oi++)
	{
		size_t	pIdx = order[oi];
		typename T::value_type value = pairs[pIdx].second;

		typename T::iterator boundIt =
			std::find(mainChain.begin(), mainChain.end(), pairs[pIdx].first);
		typename T::iterator pos =
			std::lower_bound(mainChain.begin(), boundIt, value);

		mainChain.insert(pos, value);
	}
}

/*
** Implementacion recursiva de Ford-Johnson (merge-insertion sort)
** directamente sobre T (std::vector<int> o std::deque<int>), sin
** convertirlo a ningun otro tipo de contenedor intermedio. Solo
** orquesta los pasos: emparejar, ordenar recursivamente los pares,
** construir la cadena principal, calcular el orden de Jacobsthal e
** insertar los pendientes (y el straggler si lo hay).
*/
template <typename T>
T PmergeMe::fordJohnson(T arr)
{
	typedef typename T::value_type value_type;

	if (arr.size() <= 1)
		return (arr);

	value_type	straggler = value_type();
	bool		odd;

	std::vector<std::pair<value_type, value_type> > pairs =
		makePairs<T>(arr, straggler, odd);
	sortPairsByFirst<T>(pairs);

	T mainChain = buildMainChain<T>(pairs);
	std::vector<size_t> order = jacobsthalOrder(pairs.size() - 1);

	insertPending<T>(mainChain, pairs, order);

	if (odd)
	{
		typename T::iterator pos =
			std::lower_bound(mainChain.begin(), mainChain.end(), straggler);
		mainChain.insert(pos, straggler);
	}
	return (mainChain);
}

/* --------------------------------- printing --------------------------------- */

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

	if (it == container.end())
		return (true);

	typename T::const_iterator next = it;
	++next;

	while (next != container.end())
	{
		if (*it > *next)
			return (false);
		++it;
		++next;
	}
	return (true);
}

#endif /* PMERGEME_TPP */
