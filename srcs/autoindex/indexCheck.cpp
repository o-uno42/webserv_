/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   indexCheck.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thiew <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 17:11:43 by thiew             #+#    #+#             */
/*   Updated: 2025/05/09 16:09:43 by thiew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/autoIndex.hpp"

std::string	defaultIndex(t_request &req, Server &server, int &response_code);
std::string	readIndex(std::string &index_path, int &response_code);

/** 
   @description: checks if it needs to return index page or autindex list
   @params: request, server, and refrenece to response code(since it can't be returned
   @returns: html response as std::string; NULL if the request needs to continue executing
   @calls: eitther defaultIndex of the dir or autoIndexGenerator
   @called by: who knows, somewhere in response.cpp probably
*/
std::string	dirIndexCheck(t_request &req, Server &server, int &response_code)
{
	std::string	response;
	std::cout << "XXXXXX\n";
	if (req.path_type != "dir" || (req.location.getRootLocation() == "undefined" && req.path != "/"))
	{
		std::cout << "EXITING DIR CHECK\n";
		return ("undefined output HERE\n");
	}

	std::cout << "THIS IS AUTOINDEX NUM: \n";
	std::cout << req.location.getAutoindex() << "\n";
	if (req.location.getAutoindex() == 0 || req.location.getAutoindex() == -1)
	{
		response = defaultIndex(req, server, response_code);
		return(response);

	}

	if (req.location.getAutoindex() == 1)
	{
		response = autoIndexGenerator(req, response_code);
		return (response);
	}
	return (response);
}

/*
   @description: this finds the default index if it exists in the request route
   @params: request struct, server object and reference to response code so it can be modified
   @returns: content of the index file (also modifies response code if applicable
   @called by: dirIndexCheck
*/
std::string	defaultIndex(t_request &req, Server &server, int &response_code)
{
	std::string response;
	std::ostringstream default_index;

	DIR *dir = opendir(req.full_path.c_str());
	if (!dir)
	{
		std::cerr << "error reading dir for autoIndex\n";
		default_index << "<h1>403 forbidden</h1>\n</body>";
		response_code = 403;
		return (default_index.str());
	}

	std::vector<std::string>	index_list = server.getIndexList();
	struct dirent *root;
	while ((root = readdir(dir)) != NULL)
	{
		for (std::vector<std::string>::iterator it = index_list.begin();
				it != index_list.end(); it++)
		{
			std::cout << "INSIDE default index: " << *it << "\n";
			if (root->d_name == *it)
			{
				std::string full_index_path = req.full_path + *it;
				response = readIndex(full_index_path, response_code);
				closedir(dir);
				return (response);
			}
		}
	}

	response =  "<h1>404 Not Found</h1>\n</body>";
	response_code = 404;

	closedir(dir);
	return (response);
}


std::string	readIndex(std::string &index_path, int &response_code)
{
	std::string			response;
	std::string			buff;
	std::ostringstream	response_ostream;
	std::ifstream		file;

	file.open(index_path.c_str());
	while (std::getline(file, buff))
		response_ostream << buff;
	if (file.bad())
		response_code = 500;

	return (response_ostream.str());
}
