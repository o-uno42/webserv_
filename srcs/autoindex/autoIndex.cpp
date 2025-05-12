/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thiew <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:42:01 by thiew             #+#    #+#             */
/*   Updated: 2025/05/09 16:04:57 by thiew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/autoIndex.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/utils.hpp"

/** 
   @description: generates dynamic autindex html with dir listing
   @params: request, server, and reference to response_code so it can be modified
   @returns: html formatted response as std::string
   @called by: dirIndexCheck in indexCheck.cpp
   @calls: who cares
*/
std::string	autoIndexGenerator(t_request &req, int &response_code)
{
	std::ostringstream auto_index;
	auto_index << "<html>\n<head>\n<title>Index of " 
		<< req.parsed_path << "</title>\n</head>\n<body>\n" 
		<< "<h1>Index of " << req.parsed_path << "</h1>\n<hr>\n<ul>\n";

	struct dirent *root;
	DIR *dir = opendir(req.full_path.c_str());
	if (!dir)
	{
		std::cerr << "error reading dir for autoIndex\n";
		auto_index << "<h1>403 forbidden</h1>\n</body>";
		response_code = 403;
		return (auto_index.str());
	}

	while((root = readdir(dir)) != NULL)
	{
		std::string href = req.parsed_path + std::string(root->d_name);
		if (std::string(root->d_name) == ".")
			continue ;
		if (root->d_type == DT_DIR)
			auto_index << "<li><a href=\"" << href + "/" << "\">" << std::string(root->d_name) + "/" << "</li>\n";
		if (root->d_type == DT_REG)
			auto_index << "<li><a href=\"" << href << "\">" << std::string(root->d_name) << "</li>\n";
	}

	auto_index << "</ul><hr>\n</body>\n";
	closedir(dir);

	return (auto_index.str());
}
