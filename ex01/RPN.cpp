/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 12:27:03 by alejhern          #+#    #+#             */
/*   Updated: 2026/05/19 12:27:05 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

RPN::RPN()
{
}

RPN::RPN(const RPN &other) : _stack(other._stack)
{
}

RPN &RPN::operator=(const RPN &other)
{
    if (this != &other)
        _stack = other._stack;
    return (*this);
}

RPN::~RPN()
{
}

int RPN::evaluate(const std::string &expression)
{
    for (size_t i = 0; i < expression.length(); ++i)
    {
        char token = expression[i];
        if (token == ' ')
            continue;
        else if (std::isdigit(token))
            _stack.push(token - '0');
        else if (token == '+' || token == '-' || token == '*' || token == '/')
        {
            if (_stack.size() < 2)
                throw RPNException("Invalid RPN expression: not enough operands");
            int b = _stack.top(); _stack.pop();
            int a = _stack.top(); _stack.pop();
            int result;
            switch (token)
            {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/':
                    if (b == 0)
                        throw RPNException("Invalid RPN expression: division by zero");
                    result = a / b; break;
                default: throw RPNException("Invalid RPN expression: invalid operator");
            }
            _stack.push(result);
        }
        else
            throw RPNException(std::string("Invalid token in RPN expression: ") + token);
    }
    if (_stack.size() != 1)
        throw RPNException("Invalid RPN expression: too many operands");
    return _stack.top();
}

RPN::RPNException::RPNException(const std::string &message) : std::runtime_error(message)
{
}