#include "../../includes/Everything.hpp"

/** 
*@description: Here I create the server socket
*@params: The server
*@called by: The main (for now)
*@returns: Int 0 or 1
*/
void cleanUp(std::vector<Socket> &serverSockets, std::vector<int> &client_fds, EpollHandler &epollHandler, std::vector<Server> &servers) {
    for (size_t i = 0; i < epollHandler.getEpollEvents().size(); i++) {
        if (epollHandler.getEpollEvents()[i].data.fd > 0){
            LOG_YELLOW("- - - Closing epoll events: " << epollHandler.getEpollEvents()[i].data.fd);
            epollHandler.removeSocketFromEpoll(epollHandler.getEpollEvents()[i].data.fd);
            close(epollHandler.getEpollEvents()[i].data.fd);
        }
    }
    for (size_t i = 0; i < servers.size(); i++) {
        for (size_t j = 0; j < serverSockets[i].getSocketFd().size(); ++j) {
            LOG_GREEN("- - - Closing server socket: " << serverSockets[i].getSocketFd()[j]);
            epollHandler.removeSocketFromEpoll(serverSockets[i].getSocketFd()[j]);
            close(serverSockets[i].getSocketFd()[j]);
        }
    }
    for(size_t i = 0; i < client_fds.size(); i++) {
        LOG_GREEN("- - - Closing client socket: " << client_fds[i]);
        epollHandler.removeSocketFromEpoll(client_fds[i]);
        close(client_fds[i]);
    }
    LOG_YELLOW("- - - Closing epoll FD");
    epollHandler.removeSocketFromEpoll(epollHandler.getEpollFd());
	close(epollHandler.getEpollFd());
}
