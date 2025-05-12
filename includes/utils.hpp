#ifndef UTILS_HPP
# define UTILS_HPP
#pragma once
#include <iostream>
#include "request.hpp"
#include <map>
#include "colors.hpp"
#include "Socket.hpp"
#include "EpollHandler.hpp"

#define LOG_GREEN(msg) std::cout << GREEN << msg << RESET << std::endl;
#define LOG_RED(msg) std::cout << RED << msg << RESET << std::endl;
#define LOG_YELLOW(msg) std::cout << YELLOW << msg << RESET << std::endl;
#define LOG_BLUE(msg) std::cout << BLUE << msg << RESET << std::endl;
#define LOG_MAGENTA(msg) std::cout << MAGENTA << msg << RESET << std::endl;
#define LOG_CYAN(msg) std::cout << CYAN << msg << RESET << std::endl;

std::string         readFileContents(std::ifstream& file);
std::string      	itoa(size_t num);
void               	  closeFdAndClearBuffer(int fd, std::map<int, std::string> buffer, std::map<int, int> clientToServerIndex);
void               	  handleSignal(int sig);
int					   checkValidPath(t_request &req, std::string &full_path);

// print utils
void                  printBanner();
void                  printRequestClear(const t_request &req);
void                  printRequest(const t_request &req); //TODO remove one of the two

// methods utils
std::string         getHandler(t_request &request, Server &server, std::string file_path);
std::string         postHandler(t_request &request, Server &server, std::string file_path);
std::string         deleteHandler(t_request &request, Server &server, std::string file_path);

// login utils
std::string         setHTMLPage(std::string title, std::string msg, std::string url_bg);
bool                  saveFileInDatabase(std::string filename, std::string path, const t_multi_part &part);
std::string         extractFilename(const t_multi_part &part);
std::string         loginHandler(t_request &request, Server &server);
std::string         registrationHandler(t_request &request, Server &server, std::string file_path);
std::string         saveFileHandler(t_request &request, Server &server);

//free
void cleanUp(std::vector<Socket> &serverSockets, std::vector<int> &client_fds, EpollHandler &epollHandler, std::vector<Server> &servers);

#endif
