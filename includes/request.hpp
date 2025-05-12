/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgiorgi <pgiorgi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:54:27 by tjuvan            #+#    #+#             */
/*   Updated: 2025/05/09 16:09:02 by thiew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <cstddef>
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <dirent.h>
#include <fstream>
#include <limits.h>
#include <sys/stat.h>
#include "Location.hpp"

class Server;

typedef	struct s_multi_part
{
	std::vector<std::string>	disposition_params;
	std::string					content_type;
	std::vector<std::string>	encoded_params;
	std::string 				body;

}				t_multi_part;

typedef	struct	s_request
{
	std::string							method;
	std::string							path; // the same as passed in request
	std::string							parsed_path; // path after removing
													 // query params
	std::string							full_path; //absolute path
	std::string							path_query; //query params from request line
	std::string							path_type; // "dir" or "file"
	Location							location; //location associated with request
	int									return_code; //if location is return else -1
	std::string							return_string; //if return; else empty
	std::string							protocol;
	std::string							host;
	std::string							user_agent;
	std::string							accept;
	std::string							accept_language;
	std::string							connection;
	std::string							cookies; //full cookie line
	std::vector<std::string>			cookies_vector; // cookies divided by ;
	std::string							content_type; // just the first type
	std::vector<std::string>			content_options; // options after type divided by ; 
	int				 					content_length;
	int									response_code;
	std::string							body;
	int									line_break;
	std::vector<t_multi_part>			multi_part;
	std::vector<std::string>			encoded_params;
	std::map<std::string, std::string*> params;
	std::vector<std::string>			methods;
	size_t								header_len;
	s_request()
	{
		params["Host:"] = &host;
		params["User-Agent:"] = &user_agent;
		params["Accept:"] = &accept;
		params["Accept-Language:"] = &accept_language;
		params["Connection:"] = &connection;
		params["Content-Type:"] = &content_type;
		params["Cookie:"] = &cookies;
		params["Location:"] = &return_string;
		// params["Content-Length:"] = &content_length;
		methods.resize(4);
		methods[0] = "GET";
		methods[1] = "POST";
		methods[2] = "PUT";
		methods[3] = "DELETE";
	};

}				t_request;

int 	initEmptyRequest(t_request &req, int response_code);
int		parseRequest(std::string request, t_request &parsed_req, Server &obj);
int		parseBody(std::stringstream &oss, t_request &parsed_req, Server &obj);
int 	parseMultiPart(std::stringstream &oss, t_request &parsed_req);
int		specificChecks(t_request &req, Server &server);
int		request_path_check(t_request &req, Server &server);
bool	isCgiRequest(const t_request &request);


/** 
 * @description: parses json body and stores it as a param: parsed_seq.body
 * @called by: parseBody
 * @params: original string stream, parsed_seq struct
 * @returns: error/status code and updates parsed_req struct
*/
template <typename T>
int	parseJSON(std::stringstream &oss, T &parsed_req)
{
	std::string		buff;

	std::getline(oss, buff);
	std::cout << "print buff: " << buff << std::endl;
	if (buff.empty() && (buff[0] != '{' && buff[0] != '['))
		return (400);
	parsed_req.body = buff;
	parsed_req.body += "\n";
	buff = "";

	while(std::getline(oss, buff))
	{
		parsed_req.body += buff;
		parsed_req.body += "\n";
	}
	if (!parsed_req.body.empty() && parsed_req.body[parsed_req.body.size() -1] == '\n')
		parsed_req.body.erase(parsed_req.body.size() -1);
	if (!buff.empty() && (buff[0] != '}' && buff[0] != ']'))
		return (400);
	if (oss.bad())
		return (500);
	return (100);
}

/** 
 * @description: parses plain text body of requests
*/
template <typename T>
int	parsePlain(std::stringstream &oss, T &parsed_req)
{
	std::string		buff;

	while(std::getline(oss, buff))
	{
		parsed_req.body += buff;
		parsed_req.body += "\n";
		buff = "";
	}
	if (!parsed_req.body.empty() && parsed_req.body[parsed_req.body.size() -1] == '\n')
		parsed_req.body.erase(parsed_req.body.size() -1);

	if (oss.bad())
		return (500);


	return (100);
}


/** 
 * @description: parses urlencoded bodies
*/
template <typename T>
int	parseUrlEncoded(std::stringstream &oss, T &parsed_req)
{
	std::string buff;

	while(std::getline(oss, buff, '&'))
	{
		parsed_req.body += buff + "&";
		parsed_req.encoded_params.push_back(buff);
	}
	if (!parsed_req.body.empty() && parsed_req.body[parsed_req.body.size() -1] == '\n')
		parsed_req.body.erase(parsed_req.body.size() -1);
	if (!parsed_req.body.empty() && parsed_req.body[parsed_req.body.size() -1] == '&')
		parsed_req.body.erase(parsed_req.body.size() -1);
	if (oss.bad())
		return(500);

	return (100);
}
