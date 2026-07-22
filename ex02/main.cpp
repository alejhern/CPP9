/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 00:00:00 by alejhern          #+#    #+#             */
/*   Updated: 2026/07/20 00:00:00 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

int	main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Error" << std::endl;
		return (1);
	}

	PmergeMe	pmerge;

	try
	{
		pmerge.parseArgs(argv, argc);
	}
	catch (const std::exception &e)
	{
		(void)e;
		std::cerr << "Error" << std::endl;
		return (1);
	}

	pmerge.printBefore();

	pmerge.sortVector();
	pmerge.sortDeque();

	pmerge.printAfter();
	return (0);
}
