/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoIndex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thiew <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 22:42:22 by thiew             #+#    #+#             */
/*   Updated: 2025/03/18 23:01:26 by thiew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include "request.hpp"
#include "Server.hpp"


std::string	autoIndexGenerator(t_request &req, int &response_code);
std::string	dirIndexCheck(t_request &req, Server &server, int &response_code);
