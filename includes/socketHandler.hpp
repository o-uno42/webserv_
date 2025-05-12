#include "Everything.hpp"

bool initEpollWithServers(EpollHandler &epollHandler, std::vector<Socket> &serverSockets, std::vector<Server> &servers);
bool checkDoublePort(std::vector<Server> servers);