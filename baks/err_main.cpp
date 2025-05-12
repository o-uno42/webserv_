/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjuvan <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 12:56:38 by tjuvan            #+#    #+#             */
/*   Updated: 2025/01/30 17:01:06 by tjuvan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ResponseMessage.hpp"


int err_main(int argc, char **argv)
{
	if (argc < 2)
		std::cout << "TODO - changev diss or not, but ya " << argv[0] << "\n";
	// std::cout << foo() << std::endl; //check to see if compilation works for
									 //subdirs
	std::string oss;
	oss = ResponseMessage::sendErrorPage(404);
	std::cout << oss << "\n\n" << std::endl;
	
	oss = ResponseMessage::sendErrorPage(202);
	std::cout << oss;
	return 0;
}
