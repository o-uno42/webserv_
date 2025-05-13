/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjuvan <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 18:12:33 by tjuvan            #+#    #+#             */
/*   Updated: 2025/05/12 20:32:59 by thiew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/request.hpp"
# include "../../includes/Server.hpp"

/*
 * a minimal request looks like this
 * GET / HTTP/1.1
 * Host: example.com
 *
 * we can still process it without a path and assume it want the root path
 * (/)invalid in 1.1
*/

/** 
 * EXAMPLE OF COMPLEX REQUEST
 *
 * GET /api/v1/users?page=2&sort=name&filter=status:active HTTP/1.1
 * Host: example.com
 * User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36
 * Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,;q=0.8
 * Accept-Language: en-US,en;q=0.5
 * Accept-Encoding: gzip, deflate, br
 * Cache-Control: no-cache
 * Pragma: no-cache
 * Connection: keep-alive
 * Referer: https://www.example.com/home
 * Authorization: Bearer abcdefghijklmnopqrstuvwxyz1234567890
 * X-Requested-With: XMLHttpRequest
 * DNT: 1
 * Sec-Fetch-Dest: document
 * Sec-Fetch-Mode: navigate
 * Sec-Fetch-Site: same-origin
 * Sec-Fetch-User: ?1
 * Upgrade-Insecure-Requests: 1
 */

int	parseRequestLine(std::string &request_line, t_request &parsed_req);
int	parseHeaderLine(std::stringstream &oss, std::map<std::string, std::string*>::iterator it);
int	parseHeader(std::stringstream &request_head, t_request &parsed_req);

/** 
 * @description: this parses the full request and stores it in struct
 * @params: request in string form, reference to empty request struct
 * @calls: parseRequestLine, parseHeader, ParseBody
 * @returns: error code; 100 OK, >= 400 error
*/
int	parseRequest(std::string request, t_request &parsed_req, Server &obj)
{
	std::string			buff;
	std::stringstream	oss(request);
	int					err_code = 0;

	parsed_req.line_break = 0;
	parsed_req.content_length = 0;
	parsed_req.return_code = -1;
	getline(oss, buff);
	err_code = parseRequestLine(buff, parsed_req);
	if (err_code > 100)
	{
		parsed_req.response_code = err_code;
		return (err_code);
	}
	err_code = parseHeader(oss, parsed_req);
	if (err_code > 100)
	{
		parsed_req.response_code = err_code;
		return (err_code);
	}
	err_code = parseBody(oss, parsed_req, obj);
	if (err_code > 100)
	{
		parsed_req.response_code = err_code;
		return (err_code);
	}
	err_code = specificChecks(parsed_req, obj);
	if (err_code > 100)
	{
		parsed_req.response_code = err_code;
	 	return (err_code);
	}
	parsed_req.response_code = 100;
	return (100);
}

/** 
 * @description: parses request line and returns a status code
 * @called by: parseRequest
 * @params: the first line of the request parsed request struct
 * returns: status code (100) OK, 400 >= error
 * TODO: here or some other function; path needs to be checked if valid
*/
int	parseRequestLine(std::string &request_line, t_request &parsed_req)
{
	std::stringstream	oss(request_line);
	std::string			buff;

	//PARSE METHOD
	std::getline(oss, buff, ' ');
	if (buff.empty())
		return (400);
	for (std::vector<std::string>::iterator it = parsed_req.methods.begin();
			it != parsed_req.methods.end(); ++it)
	{
		if (buff == *it)
		{
			parsed_req.method = *it;
			break ;
		}
		if (*it == "DELETE" && buff != *it)
			return (501);
	}
	if (oss.bad())
		return (500);

	//PARSE PATH
	std::getline(oss, buff, ' ');
	if (buff.empty())
		return (400);
	parsed_req.path = buff;
	//////TODO check if valid path
	if (oss.bad())
		return (500);

	//PARSE PROTOCOL
	std::getline(oss, buff, ' ');
	if (buff.empty())
		return (400);
	parsed_req.protocol = buff;
	if (parsed_req.protocol != "HTTP/1.1\r")
		return (400);
	if (oss.bad())
		return (500);

	//check if no more args
	buff = "";
	std::getline(oss, buff, ' ');
	if (!buff.empty())
		return (400);
	return (100);
}


int	parseHeader(std::stringstream &request_head, t_request &parsed_req)
{
	std::string		buff;
	std::string		word_buff;
	int				err_code = 100;

	while(std::getline(request_head, buff))
	{
		//checks for correct formatting
		if (buff == "\r")
		{
			parsed_req.line_break++;
			break;
		}
		if (buff.empty() || buff[buff.size() - 1] !=  '\r')
			return (400);
		buff.erase(buff.size() -1);

		//check for valid param
		std::stringstream oss(buff);
		std::getline(oss, word_buff, ' ');
		std::map<std::string, std::string*>::iterator it = parsed_req.params.find(word_buff);
		//if valid extract the content after param <keyword>:
		if (it != parsed_req.params.end())
			err_code = parseHeaderLine(oss, it);
		if (err_code > 100)
			return (err_code);
	}
	if (request_head.bad())
		return (500);
	return (err_code);
}

int	parseHeaderLine(std::stringstream &oss, std::map<std::string, std::string*>::iterator it)
{
	std::string	buff;

	while(std::getline(oss, buff))
		*(it->second) = buff;
	if (oss.bad())
		return (500);
	return (100);
}


