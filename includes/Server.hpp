/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeid <aeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:11:56 by aeid              #+#    #+#             */
/*   Updated: 2025/03/03 19:27:07 by aeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <limits>
# include <sstream>
# include <cstdlib>
# include <vector>
# include <sys/stat.h>
# include <unistd.h>
# include "Location.hpp"

class Location;

//     server_name blabla.com wwww.welcome.com welcome.com;
// i added the vector of aliases to the server class so we cab store the server aliases
// server aliases are : www.welcome.com welcome.com;
// maybe we can have more than one port, so we can have a vector of ports
class Server {
    
    private:
        std::vector<int> _ports;
        // int _port;
        std::string _host;
        std::string _server_name;
        std::vector<std::string> _server_aliases;
        std::vector<int> _error_code;
        std::string _error_page;
        int _client_max_body_size;
        std::string _root;
        std::string _index;
        std::vector<std::string> _index_list;
        std::vector<std::string> _allowed_methods;
        std::vector<Location> _locations;
        int _num_of_locations;
        int stringToInt(std::string str, std::string key);
        std::vector<int> stringToIntVect(std::string str, std::string key);
        std::string stringToString(std::string str, std::string key);
        std::string parseHost(std::string str);
        std::string parseServerName(std::string str);
        std::string parseRoot(std::string str);
        std::string parseIndex(std::string str);
        std::string parseErrorPage(std::string str);
        std::vector<std::string> parseAllowedMethods(std::string str);
        std::string getCurrentWorkingDirectory();

    public:
        Server();
        ~Server();
        //maybe here it is better to pass the line as a reference with const
        //it will reduce the memory usage
        void setterFunction(std::string line);
        std::vector<int> getPorts() const;
        std::string getHost() const;
        std::string getServerName() const;
        std::string getErrorPage() const;
        std::vector<int> getErrorCode() const;
        int getClientMax() const;
        std::string getRoot() const;
        std::string getIndex() const;
        std::vector<std::string> getServerAliases() const;
        std::vector<std::string> getIndexList() const;
        std::vector<std::string> getAllowedMethods() const;
        void createLocation(std::string path);
        std::vector<Location> getLocations() const;
        void addLocation(const std::string line);
        bool checkLocationPath(std::string path);
        //IMPORTANT NOTE:
        // these two constructors have to be public 
        //because vector<Server> needs them to be public(vector uses them internally)
        Server(const Server &src);
        Server &operator=(const Server &src);
        
};

# endif 