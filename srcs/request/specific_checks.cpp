/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specific_checks.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjuvan <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 17:22:47 by tjuvan            #+#    #+#             */
/*   Updated: 2025/05/12 21:34:04 by thiew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/request.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/colors.hpp"

int	allowedMethodsRequest(t_request &req, Server &server);

int	specificChecks(t_request &req, Server &server)
{
	int code = 100;
	if ((code = request_path_check(req, server)) > 100)
		return (code);
	if ((code = allowedMethodsRequest(req, server)) > 100)
		return (code);
	return (code);
}

int	allowedMethodsRequest(t_request &req, Server &server)
{
	std::vector<std::string> default_methods;
	default_methods.push_back("GET");
	default_methods.push_back("POST");
	default_methods.push_back("DELETE");

	std::vector<std::string> allowed_methods;
	std::cout << req.location.getRootLocation()<< std::endl;
	if (req.location.getLocationPath() != "undefined")
	{
		allowed_methods = req.location.getAllowedMethods();
		std::cout << "size " << allowed_methods.size() << std::endl;
	}
	else
		allowed_methods = server.getAllowedMethods();
	if (allowed_methods.empty())
		allowed_methods = default_methods;

	if (std::find(allowed_methods.begin(), allowed_methods.end(), req.method) 
			== allowed_methods.end())
	{
		std::cerr << RED << "Method: " << req.method << " not allowed at " 
			<< req.parsed_path << "\n" << RESET;
		return (405);
	}
	return (100);
}

