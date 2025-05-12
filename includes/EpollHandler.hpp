
# ifndef EPOLLHANDLER_HPP
# define EPOLLHANDLER_HPP

#include <sys/epoll.h>
# include <iostream>
# include <string>
# include <limits>
# include <sstream>
# include <cstdlib>
# include <vector>
# include "Server.hpp"
# include "Socket.hpp"

#include <sys/epoll.h>

class EpollHandler {
    private:
        int										_epoll_fd;
        std::vector<epoll_event>	_epoll_events;
		int										_epoll_num_events;
        
    public:
        EpollHandler();
        EpollHandler(const EpollHandler &src);
        EpollHandler &operator=(const EpollHandler &src);
        ~EpollHandler();
        int											getEpollFd() const;
        const std::vector<epoll_event>&		getEpollEvents() const;
		bool									createEpollFd();
        bool                                    addSocketToEpoll(int socket_fd);
		bool									modifySocketFromEpoll(int socket_fd, uint32_t event_type);
        bool                                    removeSocketFromEpoll(int socket_fd);
		int										waitEpoll(int timeout);
        class EpollException : public std::exception {
            public:
                EpollException(const char *msg) : _msg(msg) {}
                virtual const char* what() const throw() { return _msg; }
            private:
                const char *_msg;
        };

};

#endif