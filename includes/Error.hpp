/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjuvan <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:34:03 by tjuvan            #+#    #+#             */
/*   Updated: 2025/05/09 16:08:42 by thiew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <exception>
#include <sstream>
#include <cstdlib>
#include <cerrno>
#include <string>

class Error
{
	public:
		/** 
		 * @description: error function for when you don't want to stop program
		 * @params: std::string msg,optional int error, if error not passed error=-1
		 * @prints: msg + int error if passed, if not passed & errno !=0, prints errno
		 * @return: no return
		*/
		static void runningError(const std::string msg, int error=-1);

		~Error( void );


		/** 
		 *@description: EXCEPTION that allows you to pass a msg and errno code if necessary
		 *@params: nothing or std::string message or std::string message + int error code
		 *@returns: can return nothing, message or message and error code
		*/
		class FatalError: public std::exception
		{
			public:
				FatalError( std::string msg );
				FatalError( std::string msg, int errno_cpy );
				virtual	const char	*what() const throw();
				virtual				~FatalError( void ) throw();
			private:
				int					_errno_cpy;
				std::string			_msg;
		};


	private:
		Error( void );
		Error(const Error &cpy);
		Error	&operator=(const Error &other);



};
