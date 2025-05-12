#include "../../includes/Everything.hpp"

EpollHandler::EpollHandler() {
    _epoll_fd = -1;
    _epoll_events = std::vector<epoll_event>(MAX_EVENTS);
    _epoll_num_events = 0;
}

EpollHandler::EpollHandler(const EpollHandler &src) {
    *this = src;
}

EpollHandler &EpollHandler::operator=(const EpollHandler &src) {
    if (this != &src) {
        _epoll_fd = src._epoll_fd;
        _epoll_events = src._epoll_events;
        _epoll_num_events = src._epoll_num_events;
    }
    return *this;
}

EpollHandler::~EpollHandler() {
    _epoll_events.clear();
}

int EpollHandler::getEpollFd() const{
    return _epoll_fd;
}

const std::vector<epoll_event>& EpollHandler::getEpollEvents() const {
    return _epoll_events;
}

/** 
*@description: Here i create the epoll fd
*@called by: startSocket
*@returns: true or false
*/
bool EpollHandler::createEpollFd() {

    _epoll_fd = epoll_create(1);
    if (_epoll_fd < 0) {
        Error::runningError("error: epoll_create failed");
        return false;
    }
    else {
        LOG_YELLOW("- - - Epoll created FD [" << _epoll_fd << "]");
    }
    return true;
}

/** 
*@description: Here I add events to the epoll
*@params: The epoll and the socket
*@called by: startSocket
*@returns: true or false
*/
bool EpollHandler::addSocketToEpoll(int socket_fd) {
    
    struct epoll_event event = {};
    event.events = EPOLLIN ;
    event.data.fd = socket_fd;

    int result = epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, socket_fd, &event);
    if (result < 0) {
        Error::runningError("error: epoll_ctl add failed");
        return false;
    }
    return true;
}
/** 
*@description: Here I count all the events
*@params: The epoll and the socket
*@called by: startSocket
*@returns: true or false
*/
int EpollHandler::waitEpoll(int timeout) {
    
    LOG_YELLOW(DIM << ITALIC << "_waiting events (on epoll_fd: " << _epoll_fd << " ), timeout is set to "<< timeout << " ... ... ...");
    
    int num_events = epoll_wait(_epoll_fd, _epoll_events.data(), MAX_EVENTS, timeout);
    
    if (num_events == 0)
        LOG_YELLOW(DIM << ITALIC  << "_waitEpoll registered " << RED << num_events << RESET << DIM << ITALIC << YELLOW << " events")
    else
        LOG_YELLOW(DIM << ITALIC  << "_waitEpoll registered " << GREEN << num_events  << RESET << DIM << ITALIC << YELLOW << " events");
    
    if (num_events < 0 && errno != EINTR) { //EINTR indicates a signal, like Ctrl+C to close the program
        Error::runningError("error: epoll_wait failed");
    }
    _epoll_num_events = num_events;

    return num_events;
}

/** 
*@description: It changes the type of the epoll event. Usually from EPOLLIN to EPOLLOUT
*@params: The epoll and the socket
*@called by: startSocket
*@returns: true or false
*/
bool EpollHandler::modifySocketFromEpoll(int socket_fd, uint32_t event_type) {
    
    struct epoll_event event = {};
    event.events = event_type;
    event.data.fd = socket_fd;

    if (epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, socket_fd, &event) < 0) {
        Error::runningError("error: epoll_ctl failed");
        return false;
    }
    else {
        LOG_YELLOW(DIM << "Modified socket: [" << socket_fd << "] to type " << (event_type == EPOLLIN ? "EPOLLIN" : "EPOLLOUT"));
    }
    return true;
}

/** 
*@description: it removes the socket from the epoll
*@params: the fd
*@called by: startSocket
*@returns: true or false
*/
bool EpollHandler::removeSocketFromEpoll(int socket_fd) {

    if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, socket_fd, NULL) < 0) {
        Error::runningError("error: epoll_ctl failed");
        return false;
    }
    else {
        LOG_YELLOW("Removed socket: [" << socket_fd << "] from epoll");
    }
    return true;
}
