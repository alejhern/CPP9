/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 12:48:24 by alejhern          #+#    #+#             */
/*   Updated: 2026/05/19 12:48:26 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

int	main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Usage: ./PmergeMe <positive integers>" << std::endl;
		return (1);
	}

	try
	{
		PmergeMe pm;

		std::vector<int>	vec;
		std::deque<int>		deq;
		std::list<int>		lst;

		// =========================
		// PARSE
		// =========================
		pm.parse(vec, argv, argc);
		pm.parse(deq, argv, argc);
		pm.parse(lst, argv, argc);

		std::cout << "=========================\n";
		std::cout << "        BEFORE\n";
		std::cout << "=========================\n";

		std::cout << "Vector: ";
		pm.printContainer(vec);

		std::cout << "Deque : ";
		pm.printContainer(deq);

		std::cout << "List  : ";
		pm.printContainer(lst);

		// =========================
		// SORT
		// =========================
		std::cout << "\n=========================\n";
		std::cout << "        SORTING\n";
		std::cout << "=========================\n";

		pm.sort(vec);
		pm.sort(deq);
		pm.sort(lst);

		// =========================
		// AFTER
		// =========================
		std::cout << "\n=========================\n";
		std::cout << "         AFTER\n";
		std::cout << "=========================\n";

		std::cout << "Vector: ";
		pm.printContainer(vec);

		std::cout << "Deque : ";
		pm.printContainer(deq);

		std::cout << "List  : ";
		pm.printContainer(lst);

		// =========================
		// VALIDATION TEST
		// =========================
		std::cout << "\n=========================\n";
		std::cout << "      VALIDATION\n";
		std::cout << "=========================\n";

		if (pm.isSorted(vec))
			std::cout << "Vector sorted OK\n";
		else
			std::cout << "Vector NOT sorted\n";

		if (pm.isSorted(deq))
			std::cout << "Deque sorted OK\n";
		else
			std::cout << "Deque NOT sorted\n";

		if (pm.isSorted(lst))
			std::cout << "List sorted OK\n";
		else
			std::cout << "List NOT sorted\n";

	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return (0);
}