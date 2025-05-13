/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amireid <amireid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:09:44 by aeid              #+#    #+#             */
/*   Updated: 2025/05/12 21:32:43 by thiew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Location.hpp"

Location::Location(std::string path, std::string root) : _path(path), _allowed_methods(),
    _cgi_pass("undefined"), _autoindex(-1), _return(-1), _return_value("undefined"), _root_main(root) {
        // if (root != "undefined")
        //     _root_location = root + path;
        // else
        _root_location = root; // undefined :P
        // std::cout << "Root Location: " << this->_root_location << std::endl;
    }

Location::Location() : _path("undefined"), _root_location("undefined"), _allowed_methods(),
    _cgi_pass("undefined"), _autoindex(-1), _return(-1), _return_value("undefined"), _root_main("undefined") {}
Location::~Location() {}

Location::Location(const Location &src) {
    _path = src.getLocationPath();
    _root_location = src.getRootLocation();
    _allowed_methods = src.getAllowedMethods();
    _cgi_pass = src.getCgiPath();
    _autoindex = src.getAutoindex();
    _return = src.getReturn();
    _return_value = src.getReturnValue();
    _root_main = src.getRootMain();
}

Location &Location::operator=(const Location &src) {
    if (this != &src) {
        _path = src.getLocationPath();
        _root_location = src.getRootLocation();
        _allowed_methods = src.getAllowedMethods();
        _cgi_pass = src.getCgiPath();
        _autoindex = src.getAutoindex();
        _return = src.getReturn();
        _return_value = src.getReturnValue();
        _root_main = src.getRootMain();
    }
    return *this;
}

std::string Location::getLocationPath() const { return _path; }
std::string Location::getRootLocation() const { return _root_location; }
std::vector<std::string> Location::getAllowedMethods() const { return _allowed_methods; }
std::string Location::getCgiPath() const { return _cgi_pass; }
int Location::getAutoindex() const { return _autoindex; }
int Location::getReturn() const { return _return; }
std::string Location::getReturnValue() const { return _return_value; }
std::string Location::getRootMain() const { return _root_main; }

void Location::setterFunction(std::string line) {
    // std::cout << "I AM HERE: location line: " << line << std::endl;
    if (line[line.length() - 1] != ';')
        return ;
    int pos = 0;

    if (line.find("return") != std::string::npos) {
        pos = line.find("return");
        _return = redirectSet(line.substr(pos + 7), "return");
        // std::cout << "return: " << _return << std::endl;
        // std::cout << "return value: " << _return_value << std::endl;
    } else if (line.find("autoindex") != std::string::npos) {
        pos = line.find("autoindex");
        // std::cout << "autoindex line: " << line.substr(pos + 10) << std::endl;
        _autoindex = autoIndexSet(line.substr(pos + 10));
        // std::cout << "autoindex: " << _autoindex << std::endl;
    } else if (line.find("root") != std::string::npos) {
        pos = line.find("root");
        _root_location = setRootLocation(line.substr(pos + 5));
    } else if (line.find("allowed_methods") != std::string::npos) {
        pos = line.find("allowed_methods");
        _allowed_methods = parseAllowedMethods(line.substr(pos + 16));
    } else if (line.find("cgi_pass") != std::string::npos) {
        pos = line.find("cgi_pass");
        _cgi_pass = setRootLocation(line.substr(pos + 9));
    } else if (line.find("upload_store") != std::string::npos) {
        pos = line.find("upload_store");
        _root_location = setRootLocation(line.substr(pos + 13));
    }
}
int Location::redirectSet(std::string str, std::string key) {
    std::string str_num = "";
    if (key == "return") {
        // std::cout << "string recieved: " << str << std::endl;
        size_t i = 0;
        for (; i < str.length(); i++)
        {
            if (str[i] == ';' || str[i] == ' ')
                break;
            if (!std::isdigit(str[i]) && str[i] != ';')
                return -1;
        }
        str_num = str.substr(0, i);
        if (str[i] != ';')
            setReturnValue(str.substr(i + 1));
    } else
        return -1;
    std::istringstream iss(str_num);
    int result;
    iss >> result;
    if (iss.fail())
        return -1;
    if (result < -std::numeric_limits<int>::max() || result > std::numeric_limits<int>::max())
        return -1;
    return result;
}

void Location::setReturnValue(std::string str) {
    size_t i = 0;
    if (getReturnValue() == "undefined")
        _return_value = "";
    while (str[i] != ';' && i < str.length()) {
        _return_value += str[i];
        i++;
    }
}

int Location::autoIndexSet(std::string str) {
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == ' ')
            i++;
        if (str[i] == 'o' && str[i + 1] == 'n' && str[i + 2] == ';')
            return 1;
        else if (str[i] == 'o' && str[i + 1] == 'f' && str[i + 2] == 'f' && str[i + 3] == ';')
            return 0;
    }
    return -1;
}

std::string Location::getCurrentWorkingDirectory() {
    char buff[1000];
    if (getcwd(buff, 1000) == NULL)
        return "undefined";
    return std::string(buff);
}

// in the end i need to check if root location is undefined, then use root main + path
std::string Location::setRootLocation(std::string str) {
    std::string path = "";
    std::string full_path = "";
    std::string cwd = "";
    struct stat buffer;
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == ' ' || str[i] == ';')
            break;
        if (i + 1 < str.length() && str[i] == '/' && str[i + 1] == ';')
            break;
        path += str[i];
    }
    if (path == "undefined")
        return std::string("undefined");
    cwd = getCurrentWorkingDirectory();
    // std::cout << "path: " << path << std::endl;
    full_path = cwd + path;
    // std::cout << "cwd: " << cwd << std::endl;
    // std::cout << "full path: " << full_path << std::endl;
    if (stat(full_path.c_str(), &buffer) != 0) {
        std::cout << "HEY RIIIIIT" << std::endl;
        
        return std::string("undefined");
    }
    if (S_ISDIR(buffer.st_mode))
        return full_path;
    full_path = cwd + getRootMain() + path;
    // std::cout << "root main: " << getRootMain() << std::endl;
    if (stat(full_path.c_str(), &buffer) != 0)
        return std::string("undefined");
    if (S_ISDIR(buffer.st_mode))
        return full_path;
    return std::string("undefined");
}

std::vector<std::string> Location::parseAllowedMethods(std::string str) {
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
