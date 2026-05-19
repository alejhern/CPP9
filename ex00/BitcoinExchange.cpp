/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 11:56:46 by alejhern          #+#    #+#             */
/*   Updated: 2026/05/19 11:56:48 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
{
	loadExchangeRates("data.csv");
}

BitcoinExchange::BitcoinExchange(const std::string &dataFile)
{
	loadExchangeRates(dataFile);
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) : _exchangeRates(other._exchangeRates)
{
}

BitcoinExchange::~BitcoinExchange()
{
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
		_exchangeRates = other._exchangeRates;
	return (*this);
}

void BitcoinExchange::loadExchangeRates(const std::string &dataFile)
{
	double	rate;

	std::ifstream file(dataFile.c_str());
	if (!file.is_open())
		throw std::runtime_error("Could not open data file: " + dataFile);
	std::string line;
	std::getline(file, line); // skip header
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string date;
		std::string rateStr;
		if (std::getline(ss, date, ',') && std::getline(ss, rateStr))
		{
			rate = std::atof(rateStr.c_str());
			_exchangeRates[date] = rate;
		}
	}
	file.close();
}

void BitcoinExchange::processInputFile(const std::string &inputFile)
{
	double	value;

	std::ifstream file(inputFile.c_str());
	if (!file.is_open())
		throw std::runtime_error("Could not open input file: " + inputFile);
	std::string line;
	std::getline(file, line); // skip header
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string date;
		std::string valueStr;
		if (std::getline(ss, date, '|') && std::getline(ss, valueStr))
		{
			// trim espacios date
			date.erase(0, date.find_first_not_of(" \t"));
			date.erase(date.find_last_not_of(" \t") + 1);
			// trim espacios value
			valueStr.erase(0, valueStr.find_first_not_of(" \t"));
			valueStr.erase(valueStr.find_last_not_of(" \t") + 1);
			value = std::atof(valueStr.c_str());
			printBitcoinValue(date, value);
		}
	}
	file.close();
}

void BitcoinExchange::printBitcoinValue(const std::string &date, double amount)
{
	if (amount < 0)
	{
		std::cerr << "Error: not a positive number." << std::endl;
		return ;
	}

	if (amount > 1000)
	{
		std::cerr << "Error: too large a number." << std::endl;
		return ;
	}

	std::map<std::string, double>::iterator it;

	it = _exchangeRates.lower_bound(date);

	if (it == _exchangeRates.end() || it->first != date)
	{
		if (it == _exchangeRates.begin())
		{
			std::cerr << "Error: no exchange rate available for date => " << date << std::endl;
			return ;
		}
		--it;
	}

	double result = amount * it->second;

	std::cout << date << " => " << amount << " = " << std::fixed << std::setprecision(2) << result << std::endl;
}