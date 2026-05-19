/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 12:26:53 by alejhern          #+#    #+#             */
/*   Updated: 2026/05/19 12:26:58 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
# define RPN_HPP

# include <iostream>
# include <stack>
# include <stdexcept>
# include <string>

class RPN
{
  public:
    RPN();
    RPN(const RPN &other);
    RPN &operator=(const RPN &other);
    ~RPN();
    int evaluate(const std::string &expression);

    class RPNException : public std::runtime_error
    {
      public:
        RPNException(const std::string &message);
    };

  private:
    std::stack<int> _stack;
};

#endif /* RPN_HPP */