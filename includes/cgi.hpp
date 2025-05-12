/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thiew <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 12:08:40 by thiew             #+#    #+#             */
/*   Updated: 2025/05/12 19:14:38 by thiew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <iostream>
#include <algorithm>

#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>


#include "Server.hpp"
#include "request.hpp"
#include "Error.hpp"

namespace std {
    template <typename T>
    std::string to_string(T value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }
}

typedef	struct	s_cgi
{
	std::map<std::string, std::string>	env;
	char								**env_c_mtx;
	int									save_in;
	int									save_out;
	int									fd_in;
	int									fd_out;
	FILE								*file_in;
	FILE								*file_out;
	int									response_code;
}				t_cgi;

std::string cgiHandler(Server &server, t_request &req);
