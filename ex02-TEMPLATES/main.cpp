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

#define RESET  "\033[0m"
#define GREEN  "\033[32m"
#define RED    "\033[31m"
#define BLUE   "\033[34m"
#define GREY   "\033[90m"
#define BOLD   "\033[1m"

static void section(const std::string &name)
{
	std::cout << "\n"
			  << BLUE << BOLD
			  << "== " << name << " =="
			  << RESET << "\n";
}

static void result(const std::string &name, bool ok)
{
	std::cout << "  " << name << ": ";

	if (ok)
		std::cout << GREEN << "OK" << RESET;
	else
		std::cout << RED << "FAILED" << RESET;

	std::cout << "\n";
}


int	main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << RED << "Error: no arguments provided"
		          << RESET << "\n";
		std::cerr << GREY 
		          << "Usage: ./PmergeMe <positive integers>"
		          << RESET << "\n";
		return (1);
	}

	try
	{
		PmergeMe pm;

		std::vector<int> vec;
		std::deque<int>  deq;
		std::list<int>   lst;


		section("INPUT");

		pm.parse(vec, argv, argc);
		pm.parse(deq, argv, argc);
		pm.parse(lst, argv, argc);

		std::cout << "Elements: " << vec.size() << "\n";


		section("BEFORE");

		std::cout << "Vector : ";
		pm.printContainer(vec);

		std::cout << "Deque  : ";
		pm.printContainer(deq);

		std::cout << "List   : ";
		pm.printContainer(lst);


		section("SORTING");

		pm.sort(vec);
		pm.sort(deq);
		pm.sort(lst);

		std::cout << GREEN << "Completed" << RESET << "\n";


		section("AFTER");

		std::cout << "Vector : ";
		pm.printContainer(vec);

		std::cout << "Deque  : ";
		pm.printContainer(deq);

		std::cout << "List   : ";
		pm.printContainer(lst);


		section("CHECK");

		result("Vector", pm.isSorted(vec));
		result("Deque ", pm.isSorted(deq));
		result("List  ", pm.isSorted(lst));


		std::cout << "\n"
				  << GREY
				  << "--------------------------------"
				  << RESET << "\n"
				  << BOLD
				  << "PmergeMe finished successfully"
				  << RESET << "\n";

	}
	catch (const std::exception &e)
	{
		std::cerr << RED << "Error: "
		          << RESET << e.what()
		          << "\n";
		return (1);
	}

	return (0);
}