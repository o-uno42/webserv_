// #include "EpollHandler.hpp"
#include "request.hpp"
#include <sys/epoll.h>
#include <sys/types.h>
# ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>
# include <string>
# include <limits>
# include <sstream>
# include <cstdlib>
# include <vector>

#include <sys/socket.h>

class Socket {
    private:
        std::vector<int>					   _socket_fds;
        std::vector<struct sockaddr_in> _socket_addr;
        
    public:
        Socket();
        Socket(const Socket &src);
        Socket &operator=(const Socket &src);
        ~Socket();
		std::vector<int>									getSocketFd() const;
		bool								reuseSocketAddr(bool flag);
		void								initializeSocketAddr(Server server);
        bool								bindSocket();
		bool								listenSocket();

        bool                               launchingServer(Server server);
        int									startServerSocket(std::vector<Server> servers);

};

#endif
