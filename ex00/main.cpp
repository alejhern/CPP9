/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 11:16:09 by alejhern          #+#    #+#             */
/*   Updated: 2026/07/17 11:16:11 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <cstring>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"
#define BTC     "\033[38;2;247;147;26m"

// Counts display columns, not bytes: box-drawing/braille chars are
// multi-byte UTF-8 but occupy a single terminal column each.
static size_t utf8Width(const char *s)
{
	size_t width = 0;
	for (const unsigned char *p = reinterpret_cast<const unsigned char *>(s); *p; ++p)
	{
		if ((*p & 0xC0) != 0x80) // skip UTF-8 continuation bytes
			++width;
	}
	return (width);
}

static void printBanner(void)
{
	static const char *title[] =
	{
		" ██████╗ ██╗████████╗ ██████╗ ██████╗ ██╗███╗   ██╗",
		" ██╔══██╗██║╚══██╔══╝██╔════╝██╔═══██╗██║████╗  ██║",
		" ██████╔╝██║   ██║   ██║     ██║   ██║██║██╔██╗ ██║",
		" ██╔══██╗██║   ██║   ██║     ██║   ██║██║██║╚██╗██║",
		" ██████╔╝██║   ██║   ╚██████╗╚██████╔╝██║██║ ╚████║",
		" ╚═════╝ ╚═╝   ╚═╝    ╚═════╝ ╚═════╝ ╚═╝╚═╝  ╚═══╝",
		"",
		"███████╗██╗  ██╗ ██████╗██╗  ██╗ █████╗ ███╗   ██╗ ██████╗ ███████╗",
		"██╔════╝╚██╗██╔╝██╔════╝██║  ██║██╔══██╗████╗  ██║██╔════╝ ██╔════╝",
		"█████╗   ╚███╔╝ ██║     ███████║███████║██╔██╗ ██║██║  ███╗█████╗",
		"██╔══╝   ██╔██╗ ██║     ██╔══██║██╔══██║██║╚██╗██║██║   ██║██╔══╝",
		"███████╗██╔╝ ██╗╚██████╗██║  ██║██║  ██║██║ ╚████║╚██████╔╝███████╗",
		"╚══════╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝ ╚══════╝",
		"",
		NULL
	};

	static const char *btc[] =
	{
		"⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣴⣶⣾⣿⣿⣿⣿⣷⣶⣦⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀",
		"⠀⠀⠀⠀⠀⣠⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⣄⠀⠀⠀⠀⠀",
		"⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣄⠀⠀⠀",
		"⠀⠀⣴⣿⣿⣿⣿⣿⣿⣿⠟⠿⠿⡿⠀⢰⣿⠁⢈⣿⣿⣿⣿⣿⣿⣿⣿⣦⠀⠀",
		"⠀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣤⣄⠀⠀⠀⠈⠉⠀⠸⠿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀",
		"⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⠀⠀⢠⣶⣶⣤⡀⠀⠈⢻⣿⣿⣿⣿⣿⣿⣿⡆",
		"⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⠀⠼⣿⣿⡿⠃⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣷",
		"⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀⠀⢀⣀⣀⠀⠀⠀⠀⢴⣿⣿⣿⣿⣿⣿⣿⣿⣿",
		"⢿⣿⣿⣿⣿⣿⣿⣿⢿⣿⠁⠀⠀⣼⣿⣿⣿⣦⠀⠀⠈⢻⣿⣿⣿⣿⣿⣿⣿⡿",
		"⠸⣿⣿⣿⣿⣿⣿⣏⠀⠀⠀⠀⠀⠛⠛⠿⠟⠋⠀⠀⠀⣾⣿⣿⣿⣿⣿⣿⣿⠇",
		"⠀⢻⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⣤⡄⠀⣀⣀⣀⣀⣠⣾⣿⣿⣿⣿⣿⣿⣿⡟⠀",
		"⠀⠀⠻⣿⣿⣿⣿⣿⣿⣿⣄⣰⣿⠁⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠀⠀",
		"⠀⠀⠀⠙⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠋⠀⠀⠀",
		"⠀⠀⠀⠀⠀⠙⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠋⠀⠀⠀⠀⠀",
		"⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠻⠿⢿⣿⣿⣿⣿⡿⠿⠟⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀",
		NULL
	};

	// Widest title row sets the column where the coin art starts,
	// so every row lines up regardless of which block (or a blank
	// line) is being printed.
	size_t titleWidth = 0;
	for (int i = 0; title[i]; i++)
	{
		size_t w = utf8Width(title[i]);
		if (w > titleWidth)
			titleWidth = w;
	}

	std::cout << "\033[2J\033[H";

	std::cout << CYAN << BOLD;
	std::cout << "===============================================================================\n";
	std::cout << RESET;

	for (int i = 0; title[i] != NULL || btc[i] != NULL; i++)
	{
		size_t written = 0;
		if (title[i])
		{
			std::cout << CYAN << title[i] << RESET;
			written = utf8Width(title[i]);
		}
		std::cout << std::string(titleWidth - written, ' ');

		std::cout << "       ";

		if (btc[i])
			std::cout << BTC << btc[i] << RESET;

		std::cout << std::endl;
	}

	std::cout << CYAN << BOLD;
	std::cout << "===============================================================================\n";
	std::cout << RESET;

	std::cout << GREEN << BOLD
			  << "        Bitcoin Exchange Simulator - CPP09 - 42 Barcelona"
			  << RESET << std::endl << std::endl;
}

int	main(int argc, char **argv)
{
	printBanner();

	if (argc != 2)
	{
		std::cerr << RED << BOLD;
		std::cerr << "[ERROR] Invalid number of arguments\n";
		std::cerr << RESET;
		std::cerr << "Usage: " << YELLOW
				  << argv[0]
				  << " <input_file>"
				  << RESET << std::endl;
		return (1);
	}

	try
	{
		std::cout << CYAN << "[INFO] "
				  << RESET << "Loading exchange database..."
				  << std::endl;

		BitcoinExchange exchange;

		std::cout << GREEN << "[ OK ] "
				  << RESET << "Database loaded successfully."
				  << std::endl;

		std::cout << CYAN << "[INFO] "
				  << RESET << "Processing file: "
				  << YELLOW << argv[1]
				  << RESET << std::endl << std::endl;

		exchange.processInputFile(argv[1]);

		std::cout << std::endl;
		std::cout << GREEN << BOLD
				  << "[DONE] Exchange completed successfully."
				  << RESET << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << RED << BOLD
				  << "[ERROR] "
				  << RESET << e.what()
				  << std::endl;
		return (1);
	}

	return (0);
}
