#ifndef EVERYTHING_HPP
# define EVERTHING_HPP
#pragma once

#include <cstring>
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <vector>
#include <fcntl.h>

#include <netinet/in.h>

/* UTILITES*/
#include "colors.hpp"

/* CLASSES */
#include "EpollHandler.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "ResponseMessage.hpp"
#include "Error.hpp"
#include "Parser.hpp"
#include "Response.hpp"
#include "utils.hpp"
#include "cgi.hpp"
#include "request.hpp"
#include "autoIndex.hpp"
#include "socketHandler.hpp"
#include "serverSocket.hpp"

# define BUFFER_SIZE 1024
# define BODY_LIMIT 10000000000000
# define MAX_EVENTS 10
# define BACKLOG 10

#endif
