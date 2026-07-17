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
/* ------------------------------ Exceptions ------------------------------ */
 
BitcoinExchange::BitcoinException::BitcoinException(const std::string &msg) : _msg(msg)
{
}
 
BitcoinExchange::BitcoinException::~BitcoinException() throw()
{
}
 
const char *BitcoinExchange::BitcoinException::what() const throw()
{
	return (_msg.c_str());
}
 
BitcoinExchange::BadInputException::BadInputException(const std::string &line)
	: BitcoinException("Error: bad input => " + line)
{
}
 
BitcoinExchange::InvalidDateException::InvalidDateException(const std::string &line)
	: BitcoinException("Error: bad input => " + line)
{
}
 
BitcoinExchange::InvalidValueException::InvalidValueException(const std::string &line)
	: BitcoinException("Error: bad input => " + line)
{
}
 
BitcoinExchange::NegativeValueException::NegativeValueException()
	: BitcoinException("Error: not a positive number.")
{
}
 
BitcoinExchange::TooLargeValueException::TooLargeValueException()
	: BitcoinException("Error: too large a number.")
{
}
 
BitcoinExchange::NoExchangeRateException::NoExchangeRateException(const std::string &date)
	: BitcoinException("Error: no exchange rate available for date => " + date)
{
}
 
/* ------------------------- Orthodox canonical form ------------------------- */
 
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
 
/* ------------------------------- Helpers -------------------------------- */
 
void BitcoinExchange::trim(std::string &s)
{
	std::string::size_type start = s.find_first_not_of(" \t\r\n");
	if (start == std::string::npos)
	{
		s.clear();
		return ;
	}
	std::string::size_type end = s.find_last_not_of(" \t\r\n");
	s = s.substr(start, end - start + 1);
}
 
bool BitcoinExchange::isLeapYear(int year)
{
	return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}
 
// Expects strict "YYYY-MM-DD" format and checks calendar validity
// (correct number of days per month, leap years, etc).
bool BitcoinExchange::isValidDate(const std::string &date)
{
	if (date.size() != 10)
		return (false);
	if (date[4] != '-' || date[7] != '-')
		return (false);
	for (size_t i = 0; i < date.size(); ++i)
	{
		if (i == 4 || i == 7)
			continue;
		if (!std::isdigit(static_cast<unsigned char>(date[i])))
			return (false);
	}
 
	int year = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day = std::atoi(date.substr(8, 2).c_str());
 
	if (year < 0 || month < 1 || month > 12 || day < 1)
		return (false);
 
	static const int daysInMonth[12] =
		{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int maxDay = daysInMonth[month - 1];
	if (month == 2 && isLeapYear(year))
		maxDay = 29;
	if (day > maxDay)
		return (false);
	return (true);
}
 
// Accepts an optional sign, digits, and an optional single decimal point
// followed by digits. Rejects anything atof() would silently mis-parse
// (empty strings, trailing garbage, multiple dots, etc).
bool BitcoinExchange::isValidValueFormat(const std::string &valueStr)
{
	if (valueStr.empty())
		return (false);
 
	size_t i = 0;
	if (valueStr[i] == '+' || valueStr[i] == '-')
		++i;
 
	bool hasDigits = false;
	bool hasDot = false;
	for (; i < valueStr.size(); ++i)
	{
		if (std::isdigit(static_cast<unsigned char>(valueStr[i])))
		{
			hasDigits = true;
		}
		else if (valueStr[i] == '.' && !hasDot)
		{
			hasDot = true;
		}
		else
		{
			return (false);
		}
	}
	return (hasDigits);
}
 
/* ---------------------------- Loading rates ------------------------------ */
 
void BitcoinExchange::loadExchangeRates(const std::string &dataFile)
{
	double		rate;
	std::string	line;
	std::string	date;
	std::string	rateStr;
 
	std::ifstream file(dataFile.c_str());
	if (!file.is_open())
		throw std::runtime_error("Could not open data file: " + dataFile);
	std::getline(file, line); // skip header
	while (std::getline(file, line))
	{
		if (line.empty())
			continue ;
		std::stringstream ss(line);
		if (std::getline(ss, date, ',') && std::getline(ss, rateStr))
		{
			trim(date);
			trim(rateStr);
			if (!isValidDate(date) || !isValidValueFormat(rateStr))
				continue ; // skip malformed rows in the database silently
			rate = std::atof(rateStr.c_str());
			_exchangeRates[date] = rate;
		}
	}
	file.close();
}
 
/* ---------------------------- Processing input ---------------------------- */
 
void BitcoinExchange::processInputFile(const std::string &inputFile)
{
	std::ifstream file(inputFile.c_str());
	if (!file.is_open())
		throw std::runtime_error("Could not open input file: " + inputFile);
 
	std::string line;
	std::getline(file, line); // skip header
	while (std::getline(file, line))
	{
		if (line.empty())
			continue ;
		try
		{
			std::stringstream ss(line);
			std::string date;
			std::string valueStr;
 
			if (!std::getline(ss, date, '|') || !std::getline(ss, valueStr))
				throw BadInputException(line);
 
			trim(date);
			trim(valueStr);
 
			if (!isValidDate(date))
				throw InvalidDateException(line);
			if (!isValidValueFormat(valueStr))
				throw InvalidValueException(line);
 
			double value = std::atof(valueStr.c_str());
			printBitcoinValue(date, value);
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	file.close();
}
 
void BitcoinExchange::printBitcoinValue(const std::string &date, double amount)
{
	if (amount < 0)
		throw NegativeValueException();
	if (amount > 1000)
		throw TooLargeValueException();
 
	std::map<std::string, double>::iterator it;
	it = _exchangeRates.lower_bound(date);
	if (it == _exchangeRates.end() || it->first != date)
	{
		if (it == _exchangeRates.begin())
			throw NoExchangeRateException(date);
		--it;
	}
	double result = amount * it->second;
	std::cout << date << " => " << amount << " = " << std::fixed
		<< std::setprecision(2) << result << std::endl;
}