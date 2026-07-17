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
		~BitcoinExchange();
		BitcoinExchange &operator=(const BitcoinExchange &other);
 
		void processInputFile(const std::string &inputFile);
 
		// ---- Custom exceptions: each carries its own formatted message ----
		class BitcoinException : public std::exception
		{
			public:
				BitcoinException(const std::string &msg);
				virtual ~BitcoinException() throw();
				virtual const char *what() const throw();
			private:
				std::string _msg;
		};
		class BadInputException : public BitcoinException
		{
			public: BadInputException(const std::string &line);
		};
		class InvalidDateException : public BitcoinException
		{
			public: InvalidDateException(const std::string &line);
		};
		class InvalidValueException : public BitcoinException
		{
			public: InvalidValueException(const std::string &line);
		};
		class NegativeValueException : public BitcoinException
		{
			public: NegativeValueException();
		};
		class TooLargeValueException : public BitcoinException
		{
			public: TooLargeValueException();
		};
		class NoExchangeRateException : public BitcoinException
		{
			public: NoExchangeRateException(const std::string &date);
		};
 
	private:
		std::map<std::string, double>	_exchangeRates;
 
		void	loadExchangeRates(const std::string &dataFile);
		void	printBitcoinValue(const std::string &date, double amount);
 
		static bool	isValidDate(const std::string &date);
		static bool	isLeapYear(int year);
		static bool	isValidValueFormat(const std::string &valueStr);
		static void	trim(std::string &s);
};

#endif /* BITCOINEXCHANGE_HPP */