#include "Location.hpp"
#include "Server.hpp"
#include "EpollHandler.hpp"
#include <string>
typedef struct s_response{
    std::string                            content_type;
    std::vector<Location>           location;
    std::string                            server;
    std::string                            connection;
}                   t_response;
std::string			           	setHeaderResponse(t_request &req, const int &code, const std::string &content, Server &server);
std::string                     cgiSetHeaderResponse(t_request &req, const int &code, const std::string &content, Server &server);
std::string                       setResponse(t_response &res, t_request &req, const std::string &content, Server &server);
// std::string                       setErrorContent(int error_code);
std::string                       getContent(t_request &request, Server &server);
void                                sendResponse(std::map<int, std::string> &_write_buffer, int current_fd, EpollHandler &epollHandler);
bool								sendToSocket(int fd);