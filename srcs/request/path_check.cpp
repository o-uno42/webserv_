/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_check.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thiew <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:42:48 by thiew             #+#    #+#             */
/*   Updated: 2025/05/09 16:07:48 by thiew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/request.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/utils.hpp"

Location	findBestMatchingLocation(const std::string& requestPath, const Server& server);
std::string	parsed_req_path(t_request &req);
// int	checkValidPath(t_request &req, std::string &full_path);

void	hasReturnCode(t_request &req, Location &location)
{
	if (location.getReturnValue() != "undefined")
	{
		req.return_code = location.getReturn();
		req.return_string = location.getReturnValue();
	}
	else
	{
		req.return_code = -1;
		req.return_string = "";
	}
}

int	request_path_check(t_request &req, Server &server)
{
	std::string				req_path = parsed_req_path(req);
	std::string				req_args;
	std::string				location_path;
	Location 				location = findBestMatchingLocation(req_path, server);
	if (location.getRootLocation() == "undefined")
		location_path = server.getRoot();
	else if (req_path.empty())
		return (400);
	else
		location_path = location.getRootLocation();

	req.location = location;
	hasReturnCode(req, location);
	req.parsed_path = req_path;
	std::string full_path = location_path + req_path;// here the req path doesnt have the leading / that could not work in all cases
	// std::cout << YELLOW << "full path: " << full_path << std::endl;
	// std::cout << "req path: " << req_path << std::endl;
	// std::cout << "location path: " << location_path << std::endl;
	// std::cout << "return code: " << location.getReturn() << std::endl;
	// std::cout << "return string: " << location.getReturnValue() << std::endl;
	// std::cout << "query: " << req.path_query  << RESET << std::endl;


	// checking if path is file or dir -- 404 or 403 codes
	req.full_path = full_path;
	return (checkValidPath(req, full_path));
}

/** 
	@description: this parses the path withhout query params; i think?
*/
std::string	parsed_req_path(t_request &req)
{
	std::string res = "";
	size_t		i = 0;

	for (i = 0; i < req.path.size(); i++)
	{
		if (req.path[i] == '?' || req.path[i] == '#')
			break ;
	}
	if (req.path.size() > 0)
		res = req.path.substr(0,i); // take path without initial / and until query params or end
	if (req.path[i] == '?')
		req.path_query = req.path.substr(i + 1);
	return (res);
}


/**
 * @description: Finds the best matching location for a given request path
 * @params:  request path, server object
 * @return:  Pointer to the best matching location or NULL if none found
 */
Location findBestMatchingLocation(const std::string& requestPath, const Server& server)
{
	const std::vector<Location> &locations = server.getLocations();
	Location	default_location;
	Location	best_match = default_location;
	long		req_size = static_cast<long>(requestPath.size());
	long		difference = INT_MAX;

	for (std::vector<Location>::const_iterator it = locations.begin();
			it != locations.end(); it++)
	{
		std::string	loc_path = (*it).getLocationPath(); 
		// std::cout << "THIS IS LOCATION======>>>> " << loc_path << "\n";
		// std::cout << "THIS RETURN=======>>>>>> " << (*it).getReturnValue() << "\n";
		// std::cout << "THIS IS LOCATION FULL======>>>> " << (*it).getRootLocation() << "\n";
		if (requestPath.find(loc_path) == 0)
		{
			long current_difference = req_size - static_cast<long>(loc_path.size());
			if (current_difference < difference && current_difference >= 0)
			{
				difference = current_difference;
				best_match = *it;
			}
		}
	}
    return best_match;
}

