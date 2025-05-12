/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseMessage.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjuvan <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:18:28 by tjuvan            #+#    #+#             */
/*   Updated: 2025/03/24 16:47:49 by thiew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Server.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <map>
#include <string>
#include <algorithm>
// #include "Response.hpp"

class ResponseMessage
{
	public:
		//methods - getters for hashmaps
		static	std::string							getLongMessage(const int err_code);
		static	std::string							getShortMessage(const int err_code);
		//send html with error msg
		static	std::string							sendErrorPage(const int &err_code);
		static	std::string							sendErrorResponse(const int &err_code, Server &server);
		static	std::string							chooseErrorResponse(const int &err_code, Server &server);

		//deconstructor
		~ResponseMessage( void );


	private:
		//methods - @description: init of mappings
		static	std::map<int, std::string>	initErrorsShort( void );
		static	std::map<int, std::string>	initErrorsLong( void );


		//vars - @description: mapping of error codes with error msgs
		static	std::map<int, std::string>	_short_message;
		static	std::map<int, std::string>	_long_message;
		

		//constructors
		ResponseMessage( void );
		ResponseMessage(const ResponseMessage &cpy);
		ResponseMessage	&operator=(const ResponseMessage &other);
};

//utils functions
int					findlastCharIndex(const std::string &str, const std::string &substr);
std::string			intStr(int num);
