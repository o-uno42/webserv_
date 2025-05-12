/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser_Reader.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amireid <amireid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 21:08:57 by aeid              #+#    #+#             */
/*   Updated: 2025/04/02 21:55:45 by amireid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Parser.hpp"

// the location while loop that reads the location block
void Parser::LocationLoop(std::string &line, std::ifstream &file) {
    // std::cout << "ENTERED location if at line: " << line << std::endl;
    // if (line.find("location / ") == std::string::npos && line.find("location /redirect ") == std::string::npos
    //     && line.find("location /upload ") == std::string::npos && line.find("location /upload ") == std::string::npos
    //     && line.find("location /files ") == std::string::npos && line.find("location /cgi-bin ") == std::string::npos)
    //     throw FileExcep("Invalid Location");
    if (line.find("location / ") != std::string::npos) {
        // std::cout << "entered the if statement at lineeeee: " << line << std::endl;
        while (std::getline(file, line)) {
            if (line.empty() || line.find('#') != std::string::npos)
                continue;
            if (line.find('}') != std::string::npos)
                return;
            // std::cout << "entered the while loop at line: " << line << std::endl;
            this->_servers[_num_of_servers - 1].setterFunction(line);
        }}
    if (line.find("location /") != std::string::npos) {
        std::string path = "";
        //Edits on the location line.
        size_t i = line.find("location /");
        while (line[i] != '/')
            i++;
        for (; i < line.length(); i++) {
            if (line[i] == ';' || line[i] == ' ' || line[i] == '{' || line[i] == '}')
                break;
            path += line[i];
        }
        //call a method that will initialize the location vector and pass the path
        // std::cout << "PATH: " << path << std::endl;
        this->_servers[_num_of_servers - 1].createLocation(path);
        while (std::getline(file, line)) {
        // std::cout << "LOCATION: " << line << std::endl;
        if (line.empty() || line.find('#') != std::string::npos)
            continue;
        if (line.find('}') != std::string::npos)
            return;
        if (line.find("location") != std::string::npos) 
            throw FileExcep("Invalid Location");
        this->_servers[_num_of_servers - 1].addLocation(line);
    }
        if (!(this->_servers[_num_of_servers - 1].checkLocationPath(path)))
            throw FileExcep("Invalid Location");
        
    } else
        throw FileExcep("Invalid Location");
}

//the server while loop that reads the server block
void Parser::ServerLoop(std::string &line, std::ifstream &file) {
    // std::cout << "entered server if at line: " << line << std::endl;
    while (std::getline(file, line)) {
        if (line.empty() || line.find('#') != std::string::npos)
            continue;
        if (line.find('}') != std::string::npos)
            return;
        // std::cout << "entered the server while loop at line: " << line << std::endl;
        if (line.find("location") != std::string::npos) {
            // std::cout << "entered the location if statement at line: " << line << std::endl;
            LocationLoop(line, file);            
        } else
            this->_servers[_num_of_servers - 1].setterFunction(line);
}}

//the main while loop that reads the whole file
Parser::Parser(std::string path) : _num_of_servers(0) {
    if (path.empty())
        throw FileExcep("Empty Argument");
    std::ifstream file(path.c_str());
    if (!file.is_open() || !file.good() || file.fail() || file.peek() == EOF)
        throw FileExcep("Invalid File");
    std::string line = "";
    while (std::getline(file, line)) {
        // std::cout << "entered the while loop at line first: " << line << std::endl;
        if (line.empty() || line.find('#') != std::string::npos)
            continue;
        else if (line.find("server") != std::string::npos) {
            _num_of_servers++;
            _servers.push_back(Server());
            ServerLoop(line, file);
        }
        else {
            file.close();
            throw FileExcep("File is empty or t incorrect format");
        }
        // std::cout << "exited the while loop at line: " << line << std::endl;
        // std::cout << "number of servers: " << _num_of_servers << std::endl;
    }

    file.close();
}

std::vector<Server> Parser::getServers() const { return this->_servers; }

Parser::Parser() : _num_of_servers(0), _servers() {}
Parser::Parser(Parser const &src) : _num_of_servers(src._num_of_servers), _servers(src._servers) {}
Parser &Parser::operator=(Parser const &src) {
    if (this != &src) {
        _servers = src._servers;
        _num_of_servers = src._num_of_servers;
    }
    return *this;
}
Parser::~Parser() {}
Parser::FileExcep::FileExcep(std::string msg) : _msg(msg) {}
Parser::FileExcep::~FileExcep() throw() {}
const char *Parser::FileExcep::what() const throw() { return _msg.c_str(); }
