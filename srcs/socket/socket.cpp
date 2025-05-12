#include "../../includes/Everything.hpp"

Socket::Socket() {
}

Socket::Socket(const Socket &src) {
	*this = src;
}

Socket &Socket::operator=(const Socket &src) {
	if (this != &src) {
		_socket_fds = src._socket_fds;
		_socket_addr = src._socket_addr;
	}
	return *this;
}

Socket::~Socket() {
	for (size_t i = 0; i < _socket_fds.size(); ++i) {
        close(_socket_fds[i]);
    }
}

std::vector<int> Socket::getSocketFd() const {
	return _socket_fds;
}
