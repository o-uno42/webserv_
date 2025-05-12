/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseBody.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjuvan <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 15:07:45 by tjuvan            #+#    #+#             */
/*   Updated: 2025/05/09 16:07:33 by thiew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/request.hpp"
# include "../../includes/Server.hpp"
#include "../../includes/ResponseMessage.hpp"
#include "../../includes/colors.hpp"


int	prepareContent(t_request &parsed_req);
int	prepareCookies(t_request &parsed_req);
int bodyCount(std::string body_input, Server &obj);



/** 
 * @description: function that parses the body based on content type
 * @called by: parseRequest in parseRequest.cpp
 * @calls: prepareCoontent, parseJSON(request.hpp), parsePlain(request.hpp),
 * parseUrlEncoded(request.hpp), parseMultiPart(parseMultiPart.cpp)
 * @params: stringstream of the full request, struct parsed_req
 * @returns: 100 if OK; >= 400 if error 
*/
int	parseBody(std::stringstream &oss, t_request &parsed_req, Server &obj)
{
	std::string		buff;
	int				err_code = 100;

	if (bodyCount(oss.str(), obj) > 100)
		return (413);
	err_code = prepareContent(parsed_req);
	if (err_code > 100)
		return (err_code);
	err_code = prepareCookies(parsed_req);
	if (err_code > 100)
		return (err_code);
	std::cout  << CYAN << parsed_req.content_type << RESET << "\n";
	if (parsed_req.content_type == "application/json")
		err_code = parseJSON(oss, parsed_req);
	else if (parsed_req.content_type.find("multipart/form-data") == 0)
		err_code = parseMultiPart(oss, parsed_req);
	else if (parsed_req.content_type == "application/x-www-form-urlencoded")
		err_code = parseUrlEncoded(oss, parsed_req);
	else if (parsed_req.content_type == "text/plain" || parsed_req.content_type.empty())
		err_code = parsePlain(oss, parsed_req);
	return (err_code);
}

/** 
 * @description: checks if body is in compliance with max body size
 * @returns: 100 OK, 413 x
*/
int bodyCount(std::string body_input, Server &obj)
{
	// std::cout << "IM CHECKING BODY SIZE\n";
	if (static_cast<int>(body_input.size()) > obj.getClientMax())
		return (413);
	return (100);
}

/** 
 * @description: perpares the content_type and content_options by splitting
 * them; parts of parsed_req struct
 * in separate fields; content_options is a vector<string>
 * @params: reference to parsed_req sturct
 * @called by: parseBody
 * @returns: error code and sets the parsed req appropriately
*/
int	prepareContent(t_request &parsed_req)
{
	std::string		full_type = parsed_req.content_type;
	std::string		buff;
	std::stringstream	type_stream(full_type);

	std::getline(type_stream, buff, ';');
	if (buff.empty())
		return (100);
	buff.erase(std::remove(buff.begin(), buff.end(), ' '), buff.end());
	parsed_req.content_type = buff;
	buff = "";
	
	while (std::getline(type_stream, buff, ';'))
	{
		if (!buff.empty())
		{
			buff.erase(std::remove(buff.begin(), buff.end(), ' '), buff.end());
			parsed_req.content_options.push_back(buff);
		}
		buff = "";
	}
	if (type_stream.bad())
		return (500);
	return(100);
}

/** 
 * @description: perpares the cookies and cookie_vector by splitting
 * them; parts of parsed_req struct
 * in separate fields; cookie_vector is a vector<string>
 * @params: reference to parsed_req sturct
 * @called by: parseBody
 * @returns: error code and sets the parsed req appropriately
*/
int	prepareCookies(t_request &parsed_req)
{
	std::string		cookie_line = parsed_req.cookies;
	std::string		buff;
	std::stringstream	type_stream(cookie_line);

	if (cookie_line.empty())
		return (100);
	cookie_line.erase(std::remove(buff.begin(), buff.end(), ' '), buff.end());
	
	while (std::getline(type_stream, buff, ';'))
	{
		if (!buff.empty())
		{
			buff.erase(std::remove(buff.begin(), buff.end(), ' '), buff.end());
			parsed_req.cookies_vector.push_back(buff);
		}
		buff = "";
	}
	if (type_stream.bad())
		return (500);
	return(100);
}




