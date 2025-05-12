/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseMessageUtils.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjuvan <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:21:05 by tjuvan            #+#    #+#             */
/*   Updated: 2025/05/09 16:10:17 by thiew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ResponseMessage.hpp"

/** 
 * @description: works like find but instead of first index returns last
 * @params: string to check, substring which we try to find
 * @called by: SendErrorMessage in ResponseMessage.cpp
 * @returns: index of the last char of the substr, on fail returns -1
*/
int					findlastCharIndex(const std::string &str, const std::string &substr)
{
	std::string::size_type	pos = str.find(substr);
	if (pos != std::string::npos)
		return (pos + substr.length() -1);
	return (-1);
}

/** 
 * @description: turns int into string
 * @called by: SendErrorMessage in ResponseMessage.cpp
 * @returns: string representation of int
*/
std::string			intStr(int num)
{
	std::stringstream oss;
	oss << num;
	std::string res = oss.str();
	return (res);
}
