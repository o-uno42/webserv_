#ifndef SERVERSOCKET_HPP
# define SERVERSOCKET_HPP

#include "EpollHandler.hpp"

void        manageServerSocket(int i, int serverIndex, EpollHandler &epollHandler, int current_fd,
        std::map<int, int> &clientToServerIndex, std::vector<int> &client_fds);

#endif