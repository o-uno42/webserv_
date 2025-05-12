/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjuvan <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 21:35:33 by tjuvan            #+#    #+#             */
/*   Updated: 2025/05/09 16:10:02 by thiew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Error.hpp"

//private constructors
Error::Error() {};
Error::Error(const Error &cpy) {*this = cpy;};
Error	&Error::operator=(const Error &other) {if (this != &other) {;} return *this;};
//deconstructor
Error::~Error() {};

/** 
 * @description: error function for when you don't want to stop program
 * @params: std::string msg,optional int error, if error not passed error=-1
 * @prints: msg + int error if passed, if not passed & errno !=0, prints errno
 * @return: no return
*/
void Error::runningError(const std::string msg, int error)
{
	std::ostringstream oss;
    oss << msg;
    if (error != -1) {
        oss << "; error code: " << error;
    }
    if (errno > 0 && error < 1) {
        oss << "; error code: " << errno;
    }
    std::cerr << oss.str() << std::endl;
}

/** 
 *@description: EXCEPTION that allows you to pass a msg and errno code if necessary
 *@params: nothing or std::string message or std::string message + int error code
 *@returns: can return nothing, message or message and error code
*/
Error::FatalError::FatalError(std::string msg) : _errno_cpy(-1), _msg(msg){}
Error::FatalError::FatalError(std::string msg, int errno_cpy) : _errno_cpy(errno_cpy), _msg(msg)
{
	std::ostringstream oss;

	oss << _msg << "; exited with errno: " << _errno_cpy << std::endl;
	_msg = oss.str();
}
Error::FatalError::~FatalError() throw() {};
const	char	*Error::FatalError::what() const throw() { return (_msg.c_str()); }


