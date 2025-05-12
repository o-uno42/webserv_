#include "../../includes/Everything.hpp"

void manageServerSocket(int i, int serverIndex, EpollHandler &epollHandler, int current_fd,
        std::map<int, int> &clientToServerIndex, std::vector<int> &client_fds) {
	LOG_YELLOW(DIM << "- - - Event on socket FD : [" << epollHandler.getEpollEvents()[i].data.fd << "]");
	// accept new connection
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_fd =accept(current_fd, (struct sockaddr *)&client_addr, &client_len);
	if (client_fd < 0) {
		Error::runningError("error: accept failed");
		// continue;
	}

	// adding client fd to vector only if it doesn't exist yet
	if (std::find(client_fds.begin(), client_fds.end(), client_fd) == client_fds.end()) {
		client_fds.push_back(client_fd);
	}
	
	LOG_YELLOW(DIM << "Connection accepted, client FD [" << client_fd << "]");
	
	//store wich server this client is connected to
	clientToServerIndex[client_fd] = serverIndex;
	// add new client to epoll preventing duplicates
	if (epollHandler.getEpollEvents()[i].data.fd != client_fd){
		if(!epollHandler.addSocketToEpoll(client_fd)) {
			LOG_RED("- - - Failed to add client FD [" << client_fd << "] on socket FD [" << current_fd << "]");
			close(client_fd);
			// continue;
		}
		else {
			LOG_YELLOW("- - - Successfully added client FD [" << client_fd << "] on socket FD [" << current_fd << "]");
		}
	}
}