/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeid <aeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:44:41 by aeid              #+#    #+#             */
/*   Updated: 2025/02/16 17:27:57 by aeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef PARSER_HPP
# define PARSER_HPP

# include "Server.hpp"
# include <iostream>
# include <string>
# include <cstdlib>
# include <fstream>
# include <exception>
# include <vector>

# define RED "\033[1;31m"
# define RESET "\033[0m"

class Server;

//create a vector of objects made of class Server. Each object will represent a server block
class Parser {
    
    private:
        Parser();
        Parser(Parser const &src);
        Parser &operator=(Parser const &src);
        void ServerLoop(std::string &line, std::ifstream &file);
        void LocationLoop(std::string &line, std::ifstream &file);
        int _num_of_servers;
        std::vector<Server> _servers;
        //add a {} counter function..they always have to be even
        //int _parenthesis_num;
        //void parenthesisCheck(std::string str);
        
    public:
        Parser(std::string path);
        std::vector<Server> getServers() const;
        ~Parser();
    class FileExcep : public std::exception {
        private:
            std::string _msg;
        public:
            FileExcep(std::string msg);
            virtual ~FileExcep() throw();
            virtual const char *what() const throw();
    };
        
};

# endif