/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amireid <amireid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 20:48:14 by aeid              #+#    #+#             */
/*   Updated: 2025/05/12 21:32:17 by thiew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Server.hpp"
#include <iostream>
#include <vector>
#include "../includes/colors.hpp"

Server::Server() : _ports(), _host("undefined"), _server_name("undefined"), _server_aliases(),
    _error_page("undefined"), _client_max_body_size(-1),
    _root("undefined"), _index("undefined"), _index_list(), _allowed_methods(), _locations(), _num_of_locations(-1) {
        // std::cout << "Num of Locations: " << _num_of_locations << std::endl;
    }

Server::~Server() {}
Server &Server::operator=(const Server &src) {
    if (this != &src) {
        _ports = src.getPorts();
        _host = src.getHost();
        _server_name = src.getServerName();
        _server_aliases = src.getServerAliases();
        _error_page = src.getErrorPage();
        _error_code = src.getErrorCode();
        _client_max_body_size = src.getClientMax();
        _root = src.getRoot();
        _index = src.getIndex();
        _index_list = src.getIndexList();
        _allowed_methods = src.getAllowedMethods();
        _locations = src.getLocations();
        _num_of_locations = src.getLocations().size() - 1;
    }
    return *this;
}
Server::Server(const Server &src) {
    _ports = src.getPorts();
    _host = src.getHost();
    _server_name = src.getServerName();
    _server_aliases = src.getServerAliases();
    _error_page = src.getErrorPage();
    _error_code = src.getErrorCode();
    _client_max_body_size = src.getClientMax();
    _root = src.getRoot();
    _index = src.getIndex();
    _index_list = src.getIndexList();
    _allowed_methods = src.getAllowedMethods();
    _locations = src.getLocations();
    _num_of_locations = src.getLocations().size() - 1;
    // std::cout << "Num of Locationssssss: " << _num_of_locations << std::endl;
}

// we could make one getter function that return void* and then cast it to the desired type later
std::string Server::getHost() const { return _host; }
std::vector<int> Server::getPorts() const { return _ports; }
std::string Server::getServerName() const { return _server_name; }
std::vector<std::string> Server::getServerAliases() const { return _server_aliases; }
std::string Server::getErrorPage() const { return _error_page; }
std::vector<int> Server::getErrorCode() const { return _error_code; }
int Server::getClientMax() const { return _client_max_body_size; }
std::string Server::getRoot() const { return _root; }
std::string Server::getIndex() const { return _index; }
std::vector<std::string> Server::getIndexList() const { return _index_list; }
std::vector<std::string> Server::getAllowedMethods() const { return _allowed_methods; }
std::vector<Location> Server::getLocations() const { return _locations; }

void Server::createLocation(std::string path) {
    Location location(path, getRoot());
    _locations.push_back(location);
    // std::cout << "num of locations before: " << _num_of_locations << std::endl;
    _num_of_locations++;
    // std::cout << "num of locations after: " << _num_of_locations << std::endl;
}

void Server::addLocation(const std::string line) { 
    // std::cout << "location line: " << line << std::endl;
    this->_locations[_num_of_locations].setterFunction(line);
    //i am here now with code flow
    return ; 
}

bool Server::checkLocationPath(std::string path) {
    std::string rootLocationPath = this->_locations[_num_of_locations].getRootLocation();
    std::string rootMainPath = this->getRoot();
    std::string cwd = getCurrentWorkingDirectory();
    std::string fullPath;
    if (rootLocationPath == "undefined") {
        if (rootMainPath == "undefined")
            return false;
        fullPath = cwd + rootMainPath + path;
    } else
        fullPath = cwd + rootLocationPath + path;
    struct stat buffer;
    if (stat(fullPath.c_str(), &buffer) != 0)
        return false;
    if (S_ISDIR(buffer.st_mode))
        return true;
    return false;
}

void Server::setterFunction(std::string line) {
    int pos;
    if (line.find("port") != std::string::npos) {
        pos = line.find("port");
        _ports = stringToIntVect(line.substr(pos + 5), "port");
        const std::vector<int>& ports = getPorts();
        // std::cout << MAGENTA << "ports size: " << ports.size() << std::endl;
        for (size_t i = 0; i < ports.size(); ++i) {
            // std::cout << "port: " << ports[i] << std::endl;
        }
        std::cout  << RESET << std::endl;
    } else if (line.find("client_max_body_size") != std::string::npos) {
        pos = line.find("client_max_body_size");
        _client_max_body_size = stringToInt(line.substr(pos + 21), "client_max_body_size");
    } else if (line.find("host") != std::string::npos) {
        pos = line.find("host");
        // std::cout << "host line sent: " << line.substr(pos + 5) << std::endl;
        _host = stringToString(line.substr(pos + 5), "host");
        // std::cout << "host line recieved: " << _host << std::endl;
    } else if (line.find("server_name") != std::string::npos) {
        pos = line.find("server_name");
        _server_name = stringToString(line.substr(pos + 12), "server_name");
    } else if (line.find("root") != std::string::npos) {
        pos = line.find("root");
        _root = stringToString(line.substr(pos + 5), "root");
    } else if (line.find("index") != std::string::npos) {
        pos = line.find("index");
        _index = stringToString(line.substr(pos + 6), "index");
    } else if (line.find("error_page") != std::string::npos) {
        pos = line.find("error_page");
        _error_page = stringToString(line.substr(pos + 11), "error_page");
    } else if (line.find("allowed_methods") != std::string::npos) {
        pos = line.find("allowed_methods");
        _allowed_methods = parseAllowedMethods(line.substr(pos + 16));
    }
}
std::vector<int> Server::stringToIntVect(std::string str, std::string key) {
    std::vector<int> result_ports;  // Vector to hold the parsed ports
    std::string str_num = "";

    // Remove the semicolon if present at the end
    if (str[str.length() - 1] == ';') {
        str = str.substr(0, str.length() - 1);
    } else {
        if (getPorts().size() > 0) {
            getPorts().clear();
            return std::vector<int>();
        }
        return std::vector<int>();
    }

    // Process the string based on the key
    if (key == "port") {
        // std::cout << "string received: " << str << std::endl;
        
        // these lines are not needed, because I passed a string without the port keyword
        // Remove "port" keyword from the string
        // size_t pos = str.find("port");
        // if (pos != std::string::npos) {
        //     str = str.substr(pos + 5);  // Skip "port" and the following space
        // }

        // Split the string based on spaces and process each part
        std::istringstream iss(str);
        std::string port_str;

        while (iss >> port_str) {
            // Check if the port string contains only digits
            bool is_valid = true;
            for (size_t i = 0; i < port_str.length(); i++) {
                if (!std::isdigit(port_str[i])) {
                    is_valid = false;
                    break;
                }
            }

            // If valid, convert the port manually
            if (is_valid) {
                // Use std::stringstream to convert string to int (C++98 compatible)
                std::stringstream ss(port_str);
                int port;
                ss >> port;

                // Validate the port number range
                if (ss.fail() || port < 0 || port > 49151) {
                    return std::vector<int>();  // Return empty vector if invalid
                }
                result_ports.push_back(port);  // Add valid port to the result vector
            } else {
                return std::vector<int>();  // Return empty vector if invalid
            }
        }
        
    // } else if (key == "client_max_body_size") {
    //     std::cout << "string received: " << str << std::endl;
        // Handle "client_max_body_size" similarly if needed, you can extend this functionality
        // as required.
    }

    return result_ports;  // Return the vector containing valid ports
}


int Server::stringToInt(std::string str, std::string key) {
    std::string str_num = "";
    if (str[str.length() - 1] != ';')
        return -1;
    if (key == "port") {
        // std::cout << "string recieved: " << str << std::endl;
        for (size_t i = 0; i < str.length(); i++)
            if (!std::isdigit(str[i]) && str[i] != ';')
                return -1;
        str_num = str.substr(0, str.length() - 1);
    } else if (key == "client_max_body_size") {
        // std::cout << "string recieved: " << str << std::endl;
        for (size_t i = 0; i < str.length(); i++)
            if (!std::isdigit(str[i]) && str[i] != 'm' && str[i] != 'k' && str[i] != ';')
                return -1;
        if (str.find('m') != std::string::npos) {
            int pos = str.find('m');
            str_num = str.substr(0, pos) + "000000";
        } else if (str.find('k') != std::string::npos) {
            int pos = str.find('k');
            str_num = str.substr(0, pos) + "000";
        } else
            str_num = str.substr(0, str.length() - 1);
    }
    std::istringstream iss(str_num);
    int result;
    iss >> result;
    if (iss.fail())
        return -1;
    if (result < -std::numeric_limits<int>::max() || result > std::numeric_limits<int>::max())
        return -1;
    if (key == "port" && !(result >= 0 && result <= 49151))
        return -1;
    return result;
}

std::string Server::stringToString(std::string str, std::string key) {
    if (str[str.length() - 1] != ';')
        return "undefined";
    std::string str_final = str.substr(0, str.length() - 1);
    if (key == "host")
        return parseHost(str_final);
    if (key == "server_name")
        return parseServerName(str_final);
    if (key == "root")
        return parseRoot(str_final);
    if (key == "index")
        return parseIndex(str_final);
    if (key == "error_page")
        return parseErrorPage(str_final);
    return "undefined";
}

std::vector<std::string> Server::parseAllowedMethods(std::string str) {
    std::vector<std::string> allowed_methods;
    if (str[str.length() - 1] != ';')
        return std::vector<std::string>();
    std::string str_final = str.substr(0, str.length() - 1);
    //std::cout << "allowed methods: " << str_final << std::endl;
    for (size_t i = 0; i < str_final.length(); i++) {
        if (str_final[i] == ' ')
            i++;
        std::string tmp = "";
        while (i < str_final.length() && str_final[i] != ' ') {
            tmp += str_final[i];
            i++;
        }
        // if (tmp == "GET" || tmp == "POST" || tmp == "DELETE")
		allowed_methods.push_back(tmp);
        // else
        //     allowed_methods.clear();
    }
    return allowed_methods;
}

std::string Server::parseHost(std::string str) {
    if (str == "localhost" || str == "INADDR_ANY" || str == "INADDR_LOOPBACK" 
        || str == "INADDR_BROADCAST" || str == "INADDR_NONE" || str == "INADDR_ALL" 
        || str == "INADDR_GROUP" || str == "INADDR_ANY_GROUP" || str == "INADDR_MAX_LOCAL_GROUP")
            return str;
        if (!(str.length() >  6 && str.length() < 16))
            return "undefined";
        int counter = 0;
        int pre_counter = 0;
        int num = 0;
        for (size_t i = 0; i < str.length(); i++) {
            if (!std::isdigit(str[i]) && str[i] != '.') {
                return "undefined";
            }
            if (str[i] == '.') {
                if (str[i + 1] == '\0')
                    return "undefined";
                num = atoi(str.substr(pre_counter, i - pre_counter).c_str());
                // if (!(num >= 0 && num <= 255) || ((i - pre_counter) > 3 && (i - pre_counter) < 0) || (i - pre_counter) == 0)
                if (!(num >= 0 && num <= 255) || ((i - pre_counter) > 3) || (i - pre_counter) == 0)
                    return "undefined";
                counter++;
                pre_counter = i + 1;
            }
            if (counter > 3)
                return "undefined";
        }
        return str.substr(0, str.length());
}

// double check this, maybe it is not needed to have string variable, and use vector right away.. for this and the index.
std::string Server::parseServerName(std::string str) {
    std::string str_final = "";
    std::string tmp = "";
    size_t i = 0;
    while (i < str.length() && str[i] != ' ') {
        str_final += str[i];
        i++;
    }
    // std::cout << "server name: " << str_final << std::endl;
    // std::cout << "server name length: " << str_final.length() << std::endl;
    for (; i < str.length(); i++) {
        if (str[i] == ' ')
            i++;
        while (i < str.length() && str[i] != ' ') {
            tmp += str[i];
            i++;
        }
        // std::cout << "tmp: " << tmp << std::endl;
        if (!tmp.empty())
            _server_aliases.push_back(tmp);
        tmp = "";
    }
    return str_final;
}
// root should always start with a / and end never end with a /..use / to seperate directories
// also check if the root path is valid and exists

std::string Server::getCurrentWorkingDirectory() {
    char buff[1000];
    if (getcwd(buff, 1000) == NULL)
        return "undefined";
    return std::string(buff);
}

std::string Server::parseRoot(std::string str) {
    struct stat buffer;
    std::string full_path;
    
    // if (str == "html")
    //     return "etc/nginx/html";
    std::string cwd = getCurrentWorkingDirectory();
    // std::cout << "cwd: " << cwd << std::endl;
    if (str.empty() || str[0] != '/' || str[str.length() - 1] == '/')
        return "undefined";
    full_path = cwd + str;
    if (stat(full_path.c_str(), &buffer) != 0)
        return "undefined";
    if (S_ISDIR(buffer.st_mode)) 
        return full_path;
    return "undefined";
}

std::string Server::parseIndex(std::string str) {
    std::string str_final = "";
    std::string tmp = "";
    size_t i = 0;
    // while (i < str.length() && str[i] != ' ') {
    //     str_final += str[i];
    //     i++;
    // }
    //this error is not needed, because if this is wrong file..i should go and check the next
    // if (str_final.find(".html") == std::string::npos && str_final.find(".htm") == std::string::npos && str_final.find(".php") == std::string::npos)
    //     return "undefined";
    for (; i < str.length(); i++) {
        if (str[i] == ' ')
            i++;
        while (i < str.length() && str[i] != ' ') {
            tmp += str[i];
            i++;
        }
        // std::cout << "tmp: " << tmp << std::endl;
        if (!tmp.empty())
            _index_list.push_back(tmp);
        tmp = "";
    }
    return str_final;
}

std::string Server::parseErrorPage(std::string str) {
    std::string full_path;
    size_t pos = str.find("/");
    std::string error_code = "";
    if (pos == std::string::npos)
        return "undefined";
    for (size_t i = 0; i < pos; i++) {
        while (str[i] != ' ' && i < pos) {
            error_code += str[i];
            if (!std::isdigit(str[i]))
                return "undefined";
            i++;
        }
        if (error_code.empty() || error_code.length() != 3)
            return "undefined";
        _error_code.push_back(atoi(error_code.c_str()));
        error_code = "";
    }
    // std::cout << "rest of the string: " << str.substr(pos) << std::endl;
    if (getRoot() == "undefined") {
        full_path = getCurrentWorkingDirectory() + "/public" + str.substr(pos);
    } else if (getRoot() != "undefined") {
        full_path = getRoot() + str.substr(pos);
    }
        // std::cout << "full path: " << full_path << std::endl;
        struct stat buffer;
        if (stat(full_path.c_str(), &buffer) != 0) {
            // std::cout << "error page not found" << std::endl;
            return "undefined";
        }
        if (S_ISREG(buffer.st_mode))
            return full_path;   
    return full_path;
}

//error handle...if the port line is written out of scope. seg fault
