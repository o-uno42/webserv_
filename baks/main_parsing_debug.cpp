/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parsing_debug.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeid <aeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 12:56:38 by tjuvan            #+#    #+#             */
/*   Updated: 2025/03/07 20:12:07 by aeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
# define DEFAULT_PATH "default_path/default.conf"
# include "../includes/Parser.hpp"

std::string handle_input (int argc, char **argv) {
	if (argc == 1)
		return DEFAULT_PATH;
	if (argc == 2)
		return argv[1];
	if (argc > 2) {
		std::cerr << "Invalid number of arguments" << std::endl;
		exit (1);
	}
	return "";
}


int main(int argc, char **argv)
{
	std::string path = handle_input(argc, argv);
	std::vector<Server> servers_vector;
	try {
		Parser parser(path);
		std::cout << "will close the file now" << std::endl;
		servers_vector = parser.getServers();
	} catch (const Parser::FileExcep &e) {
		std::cout << "will close the file noww" << std::endl;
		std::cerr << RED << e.what() << RESET << std::endl;
		return 1;
	}
	// std::cout << "Server port: " <<  servers_vector[0].getPorts() << std::endl;
	const std::vector<int> &_ports = servers_vector[0].getPorts();
	if (!_ports.empty()) {
		std::cout << "Server port: ";
		for (size_t i = 0; i < _ports.size(); ++i) {
			std::cout << _ports[i];
			if (i < _ports.size() - 1) {
				std::cout << ", ";
			}
		}
		std::cout << std::endl;
	} else {
		std::cout << "Server ports: None" << std::endl;
	}
	std::cout << "Server host: " <<  servers_vector[0].getHost() << std::endl;
	std::cout << "Server name: " <<  servers_vector[0].getServerName() << std::endl;
	std::cout << "Server root: " <<  servers_vector[0].getRoot() << std::endl;
	const std::vector<std::string> &aliases = servers_vector[0].getServerAliases();
	if (!aliases.empty()) {
		std::cout << "Server aliases: ";
		for (size_t i = 0; i < aliases.size(); ++i) {
			std::cout << aliases[i];
			if (i < aliases.size() - 1) {
				std::cout << ", ";
			}
		}
		std::cout << std::endl;
	} else {
		std::cout << "Server aliases: None" << std::endl;
	}
	
	const std::vector<std::string> &methods = servers_vector[0].getAllowedMethods();
	if (!methods.empty()) {
		std::cout << "Allowed methods: ";
		for (size_t i = 0; i < methods.size(); ++i) {
			std::cout << methods[i];
			if (i < methods.size() - 1) {
				std::cout << ", ";
			}
		}
		std::cout << std::endl;
	} else {
		std::cout << "Allowed methods: None" << std::endl;
	}

	const std::vector<int> &error_codes = servers_vector[0].getErrorCode();
	if (!error_codes.empty()) {
		std::cout << "Error codes: ";
		for (size_t i = 0; i < error_codes.size(); ++i) {
			std::cout << error_codes[i];
			if (i < error_codes.size() - 1) {
				std::cout << ", ";
			}
		}
		std::cout << std::endl;
	} else {
		std::cout << "Error codes: None" << std::endl;
	}

	const std::vector<std::string> &indexes = servers_vector[0].getIndexList();
	if (!indexes.empty()) {
		std::cout << "Index list: ";
		for (size_t i = 0; i < indexes.size(); ++i) {
			std::cout << indexes[i];
			if (i < indexes.size() - 1) {
				std::cout << ", ";
			}
		}
		std::cout << std::endl;
	} else {
		std::cout << "index list: None" << std::endl;
	}
	
	std::cout << "Error page: " <<  servers_vector[0].getErrorPage() << std::endl;
	std::cout << "Client max body size: " <<  servers_vector[0].getClientMax() << std::endl;
	std::cout << "Root: " <<  servers_vector[0].getRoot() << std::endl;
	std::cout << "Index: " <<  servers_vector[0].getIndex() << std::endl;
	std::vector<Location> locations = servers_vector[0].getLocations();
	std::cout << "Number of locations: " << locations.size() << std::endl;
	// get locations
	for (size_t i = 0; i < locations.size(); ++i) {
		std::cout << "Location path: " << locations[i].getLocationPath() << std::endl;
		std::cout << "Location root: " << locations[i].getRootLocation() << std::endl;
		std::cout << "Location root main: " << locations[i].getRootMain() << std::endl;
		const std::vector<std::string> &methods = locations[i].getAllowedMethods();
		if (!methods.empty()) {
			std::cout << "Allowed methods: ";
			for (size_t j = 0; j < methods.size(); ++j) {
				std::cout << methods[j];
				if (j < methods.size() - 1) {
					std::cout << ", ";
				}
			}
			std::cout << std::endl;
		} else {
			std::cout << "Allowed methods: None" << std::endl;
		}
		std::cout << "Cgi path: " << locations[i].getCgiPath() << std::endl;
		std::cout << "Autoindex: " << locations[i].getAutoindex() << std::endl;
		std::cout << "Return: " << locations[i].getReturn() << std::endl;
		std::cout << "Return value: " << locations[i].getReturnValue() << std::endl;
	}
	return 0;
}
