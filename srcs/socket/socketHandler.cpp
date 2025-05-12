#include "../../includes/Everything.hpp"

/** 
*@description: Check if there is a double port
*@params: The server
*@called by: startSocket
*@returns: True, false if it fails
*/
bool checkDoublePort(std::vector<Server> servers) {
    std::vector<int> ports;
    for (size_t i = 0; i < servers.size(); i++) {
        for (size_t j = 0; j < servers[i].getPorts().size(); j++) {
            if (std::find(ports.begin(), ports.end(), servers[i].getPorts()[j]) != ports.end()) {
                Error::runningError("error: double port detected");
                return false;
            }
            ports.push_back(servers[i].getPorts()[j]);
        }
    }
    return true;
}

/** 
*@description: Here it is the main loop for the sockets
*@params: The server
*@called by: startSocket
*@returns: True, false if it fails
*/
bool initEpollWithServers(EpollHandler &epollHandler, std::vector<Socket> &serverSockets, std::vector<Server> &servers) {
    if(!epollHandler.createEpollFd()){return false;}; // creating epoll fd
	for (size_t i = 0; i < servers.size(); i++) {
		for (size_t j = 0; j < serverSockets[i].getSocketFd().size(); ++j) {
            if (serverSockets[i].getSocketFd()[j] > 0) {
                if (!epollHandler.addSocketToEpoll(serverSockets[i].getSocketFd()[j])) {return false;} 
                else{
                    LOG_YELLOW("Added socket FD [" << serverSockets[i].getSocketFd()[j] << "] to epoll FD [" << epollHandler.getEpollFd() << "]");
                }
            }
            else
                Error::runningError("Couldn't add socket FD [" + std::to_string(serverSockets[i].getSocketFd()[j]) + "]");
        }
	}
    return true;
}

/** 
*@description: Here it is the main loop for the sockets
*@params: The server
*@called by: startSocket
*@returns: True, false if it fails
*/
bool Socket::launchingServer(Server server) {

    const std::vector<int>& ports = server.getPorts();
    _socket_fds.clear();
    _socket_addr.clear();
    _socket_addr.resize(ports.size());

    std::cout << std::endl;
    LOG_GREEN("- - - Lauching server [" << server.getServerName() << "] on " << ports.size() << " ports");
    
    for (size_t i = 0; i < ports.size(); ++i) { // create a socket for each port
        int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd < 0) {
            Error::runningError("error: socket creation failed for port " + std::to_string(ports[i]));
            close(socket_fd);
        }
        else {
            LOG_GREEN("Created socket FD [" << socket_fd << "] for port: " << ports[i]);
        }
        
        // set reuse addr for each socket
        int opt = 1; // flag: true
        if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            Error::runningError("error: setsockopt failed for socket " + std::to_string(socket_fd));
            close(socket_fd);
        }
        else{
            LOG_GREEN(DIM << "Set to REUSABLE");
        }
        
        // _socket_fds.push_back(socket_fd);
        
        // init  the address structure
        _socket_addr[i].sin_family = AF_INET;
        _socket_addr[i].sin_port = htons(ports[i]);
        _socket_addr[i].sin_addr.s_addr = INADDR_ANY;
        
        LOG_GREEN(DIM << "Binding socket " << socket_fd << " to port      : " << ports[i]);
                  
        if (bind(socket_fd, (struct sockaddr*)&_socket_addr[i], sizeof(_socket_addr[i])) < 0) {
            Error::runningError("error: bind failed for port " + std::to_string(ports[i]));
            close(socket_fd);
            continue;
        }
        
        if (listen(socket_fd, BACKLOG) < 0) { //listen on the socket
            Error::runningError("error: listen failed for port " + std::to_string(ports[i]));
            close(socket_fd);
            continue;
        }
        // add the socket fd to vect
        _socket_fds.push_back(socket_fd);
        LOG_GREEN(DIM << "Socket " << socket_fd << " listening on port    : " << ports[i]);
        LOG_GREEN("- Finished initializing socket FD [" << socket_fd << "] ; Port : [" << ports[i] << "]");
    }
    return true;
}
