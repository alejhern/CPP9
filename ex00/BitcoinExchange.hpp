/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 11:55:03 by alejhern          #+#    #+#             */
/*   Updated: 2026/05/19 11:55:05 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <cstdlib>
# include <fstream>
# include <iomanip>
# include <iostream>
# include <map>
# include <sstream>
# include <stdexcept>

class BitcoinExchange
{
  public:
	BitcoinExchange();
	BitcoinExchange(const std::string &dataFile);
	BitcoinExchange(const BitcoinExchange &other);
	BitcoinExchange &operator=(const BitcoinExchange &other);
	~BitcoinExchange();
	void processInputFile(const std::string &inputFile);
	void printBitcoinValue(const std::string &date, double amount);

  private:
	std::map<std::string, double> _exchangeRates;
	void loadExchangeRates(const std::string &dataFile);
};

#endif /* BITCOINEXCHANGE_HPP */