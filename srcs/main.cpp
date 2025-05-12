/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeid <aeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 12:56:38 by tjuvan            #+#    #+#             */
/*   Updated: 2025/03/19 19:04:44 by aeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
#include <iostream>
# define DEFAULT_PATH "config/default_path/default.conf"
# include "../includes/Everything.hpp"


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
	Socket serverSocket;
	std::string path = handle_input(argc, argv);
	std::vector<Server> servers_vector;
	try {
		if (argc == 2)
		{
			Parser parser(argv[1]);
			servers_vector = parser.getServers();
		}
		else
		{
			Parser parser(path);
			servers_vector = parser.getServers();
		}
	} catch (const Parser::FileExcep &e) {
		std::cerr << BLUE << e.what() << RESET << std::endl;
		return 1;
	}
	printBanner();
	serverSocket.startServerSocket(servers_vector);
	return 0;
}
