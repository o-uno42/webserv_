/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseMessage.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjuvan <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:18:05 by tjuvan            #+#    #+#             */
/*   Updated: 2025/05/09 16:10:10 by thiew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ResponseMessage.hpp"
#include <cstring>
#include <string>
#include <sys/socket.h>

/** 
 * @description: two mappings that have error codes saved, with corresponding
 * messages
 * @example: [404] = "Not found" - short message
*/
std::map<int, std::string>	ResponseMessage::_short_message = ResponseMessage::initErrorsShort();
std::map<int, std::string>	ResponseMessage::_long_message = ResponseMessage::initErrorsLong();

//constructors - dont use
ResponseMessage::ResponseMessage() {};
ResponseMessage::ResponseMessage(const ResponseMessage &cpy) {*this = cpy;};
ResponseMessage	&ResponseMessage::operator=(const ResponseMessage &other) {if (this != &other) {;} return *this;};
ResponseMessage::~ResponseMessage() {};

//getters for hashmaps
std::string					ResponseMessage::getShortMessage(const int err_code)
{ return (_short_message[err_code]); }
std::string					ResponseMessage::getLongMessage(const int err_code)
{ return (_long_message[err_code]); }

/** 
 * @description: checks in the map for passed response code and returns
 * appropriate html file
 * @params: err_code - here you pass the appropriate response code (check
 * hashmaps for available codes
 * @calls: ifstream, getline, insert, findLastCharIndex, intStr
 * @called by: whichever part needs to send the html in response
 * @returns: a string with the appropriately modified error.html file content
*/
std::string					ResponseMessage::sendErrorPage(const int &err_code)
{
	std::string			short_message = _short_message[err_code];
	std::string			long_message = _long_message[err_code];
	std::stringstream 	oss;
	std::string			insertion;
	std::string			buff;
	int					idx = -1;

	std::ifstream	html_page("public/error_page/error.html");
	if (!html_page.is_open()) {
		oss << "Error: Unable to open error page template.";
		return (oss.str());
	}

	while(std::getline(html_page, buff))
	{
		if ((idx = findlastCharIndex(buff, "<title>")) >= 0)
		{
			insertion = intStr(err_code) + " " + short_message;
			buff.insert(idx + 1, insertion);
		}
		else if ((idx = findlastCharIndex(buff, "<h1>")) >= 0)
		{
			insertion = intStr(err_code) + " " + short_message;
			buff.insert(idx + 1, insertion);
		}
		else if ((idx = findlastCharIndex(buff, "<h2>")) >= 0)
		{
			insertion = intStr(err_code) + ": " + long_message;
			buff.insert(idx + 1, insertion);
		}
		oss << buff << "\n";
	}

	if (html_page.bad()) {
		(oss).str("");
		oss << "Error: An error occurred while reading the error page template.";
	}

	return (oss.str());
}

/** 
 * @description: generates proper error page based on error code sent
 * @params: error code, server struct
 * @returns: html in std::string form
 * @calls: chooseErrorResponse
*/
std::string ResponseMessage::sendErrorResponse(const int &err_code, Server &server)
{
	std::stringstream ss;
	std::stringstream len;
	ss << err_code;
	std::string err_code_str = ss.str();
	std::string body;
	
	body = chooseErrorResponse(err_code, server);
	len << body.length();
	std::string length = len.str();

	// Create the error message dynamically with the error code
	// std::string error_msg = "HTTP/1.1 " + err_code_str + " " + _short_message[err_code] + "\r\n"
	// 						"Content-Type: text/html\r\n"
	// 						"Content-Length: " + length + "\r\n"
	// 						"Server: " + server.getServerName() + "\r\n"
	// 						"Connection: keep-alive\r\n\r\n"
	// 						+ body;
	std::string error_msg = body;


	return error_msg;
}

/** 
 * @description: checks if error page is set in config otherwise returns default
 * @params: error code and server struct
 * @calls: server vector and config page or sendErrorPage method
 * @returns: html page as string which is then used as body of response
*/

std::string							ResponseMessage::chooseErrorResponse(const int &err_code, Server &server)
{
	std::string	body;
	std::vector<int> server_err_codes = server.getErrorCode();
	if (std::find(server_err_codes.begin(), server_err_codes.end(), err_code) != server_err_codes.end())
	{
		std::string error_path = server.getErrorPage();
		std::string buff;
		std::ifstream error_page(error_path.c_str());
		if (!error_page.is_open())
			return (sendErrorPage(500));
		while (std::getline(error_page, buff))
			body +=buff;
		if(error_page.bad())
			return (sendErrorPage(500));
		error_page.close();
	}
	else
		body = sendErrorPage(err_code);
	return (body);
}
