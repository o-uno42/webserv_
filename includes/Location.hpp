/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeid <aeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:35:35 by aeid              #+#    #+#             */
/*   Updated: 2025/04/03 20:24:13 by thiew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef LOCATION_HPP
# define LOCATION_HPP

# include "Server.hpp"

// enum LocationType {
//     ROOT,
//     REDIRECT,
//     FILES,
//     CGI,
//     UPLOAD,
//     UNKNOWN
// };
//Root is:
// if _root_location is undefined then root is _root_main + _path
// else root is _root_location + _path
// always check if any of both is undefined (DONE ALL)
// _root_location should be always checked for undefined

class Location {
    private:
        std::string _path;
        std::string _root_location;
        std::vector<std::string> _allowed_methods;
        std::string _cgi_pass;
        int _autoindex;
        int _return;
        std::string _return_value;
        std::string _root_main;
        
    public :
        Location();
        Location(std::string path, std::string root);
        ~Location();
        Location &operator=(const Location &src);
        Location(const Location &src); 
        void setterFunction(std::string line);
        int redirectSet(std::string str, std::string key);
        void setReturnValue(std::string str);
        int autoIndexSet(std::string str);
        std::string getLocationPath() const;
        std::string getRootLocation() const;
        std::string getRootMain() const;
        std::vector<std::string> getAllowedMethods() const;
        std::string getCgiPath() const;
        int getAutoindex() const;
        int getReturn() const;
        std::string getReturnValue() const;
        std::string getCurrentWorkingDirectory();
        std::string setRootLocation(std::string str);
        std::vector<std::string> parseAllowedMethods(std::string str);
        
};

# endif
