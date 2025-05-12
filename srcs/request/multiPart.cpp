/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiPart.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjuvan <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 15:35:37 by tjuvan            #+#    #+#             */
/*   Updated: 2025/05/09 16:07:24 by thiew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/request.hpp"
#include "../../includes/colors.hpp"

int	multiPartParams(std::string &form_data, t_request &parsed_req);
int	checkBodyPart(t_request &parsed_req);



/** 
 * @description: here we parse the body of multipart req
 * 				each individual part of body is processed and stored into a
 * 				separate struct and this struct are stored in a vector in
 * 				parse_req struct / check request.hpp to see structs
 * @params: stringstream of the request, parsed_req struct
 * @called by: parseBody in parseBody.cpp
 * @calls: multiPartParams
 * @returns: 100 if OK; >= 400 if error 
*/
int parseMultiPart(std::stringstream &oss, t_request &parsed_req)
{
	int						err_code = 100;
	std::string				buff;
	std::string				form_data;
	std::string				boundary = (!parsed_req.content_options.empty() 
			? parsed_req.content_options[0] : "");
	std::string				final_boundary;
	std::string::size_type	it = boundary.find("=");

	if (boundary.empty() || it == std::string::npos)
		return (400);

	boundary = boundary.substr(it + 1);
	// if (boundary == "") 
	if (boundary.empty()) 
		return (400);
	boundary.insert(0, "--");
	boundary.insert(boundary.size(), "\r");
	final_boundary = boundary;
	final_boundary.insert(boundary.size() - 1, "--");
	// boundary = "--" + boundary;
	// final_boundary = boundary + "--";
	//until here we extracted boundary value to check it against the req body
	// std::cout << RED << "boundary: " << boundary << RESET << std::endl;

	//skip first boundary
	std::getline(oss, buff);
	if (boundary != buff)
		return (400);
	parsed_req.body = buff + "\n";
	buff = "";

	//loop that checks each individual body part
	while (std::getline(oss, buff))
	{
		if (buff == boundary)
			err_code = multiPartParams(form_data, parsed_req);
		else if (buff == final_boundary)
		{
			err_code = multiPartParams(form_data, parsed_req);
			break;
		}
		else
			form_data += buff + "\n";
		if (err_code > 100)
			return (err_code);
	}
	if (oss.bad())
		return (500);

	err_code = checkBodyPart(parsed_req);
	return (err_code);
}


/** 
 * @description: checks each individual part of the body of multipart request
 * 				saves it into a struct t_multi_part which is then pushed to
 * 				parsed_req.multi vector (1st body part is in index 0, 2nd is
 * 				[1],...
 * @called by: parseMultiPart
 * params: the whole bodypart saved as string, parsed_req struct
 * @returns: 100 if OK; >= 400 if error 
*/
int	multiPartParams(std::string &form_data, t_request &parsed_req)
{
	std::string			buff;
	std::string			word_buff;
	std::stringstream	oss(form_data);
	t_multi_part		multi;

	//search the part-string for params
	parsed_req.body += form_data;
	while (std::getline(oss, buff))
	{
		//puts all the options of the disposition into a vector
		std::stringstream	line_oss(buff);
		if (buff.find("Content-Disposition") != std::string::npos)
		{
			std::getline(line_oss, word_buff, ' ');
			while(std::getline(line_oss, word_buff, ' '))
			{
				std::string::size_type it = word_buff.find(";");
				if (it != std::string::npos)
					word_buff.erase(it);
				multi.disposition_params.push_back(word_buff);
			}
		}
		//extracts content type
		else if (buff.find("Content-Type") != std::string::npos)
		{
			std::getline(line_oss, word_buff, ' ');
			std::getline(line_oss, word_buff);
			/* word_buff.erase(std::remove(word_buff.begin(), word_buff.end(), ' '), */
			/* 		word_buff.end()); */
			if (word_buff[word_buff.size() -1] == '\r')
				word_buff.erase(word_buff.end() - 1);
			multi.content_type = word_buff;
		}
		else if (buff == "\r")
			;
		//parse body of body part
		else
		{
			multi.body += buff + "\n";
			while (std::getline(oss, buff))
			{
				multi.body += buff + "\n";
				if (buff == "\r")
					break;
			}
			std::string::size_type pos = multi.body.rfind("\n");
			if (pos != std::string::npos)
				multi.body.erase(pos);
		}
	}
	if (multi.content_type.empty() || multi.disposition_params.empty())
		return (400);
	buff = "";

	if (oss.bad())
		return (500);
	//pushing it into the vector multi_part in parse_req
	parsed_req.multi_part.push_back(multi);
	//reset and ready the form_data for next iteration
	form_data = "";
	return (100);
}

/** 
 * @description: checks if bodies of each body part are correctly formatted
 * @called by: parseMultiPart
 * @calls: parseJSON(request.hpp), parsePlain(request.hpp),
 * parseUrlEncoded(request.hpp)
 * @params: parsed_req, which then is checked for each multi_part in
 * vector<multi_part>
 * @returns: 100 if OK; >= 400 if error 
*/
int	checkBodyPart(t_request &parsed_req)
{
	int		err_code = 100;

	for (std::vector<t_multi_part>::iterator it = parsed_req.multi_part.begin();
			it != parsed_req.multi_part.end(); ++it)
	{
		std::stringstream	oss(it->body);
		if (it->content_type == "application/json")
		{
			it->body = "";
			err_code = parseJSON(oss, *it);
		}
		else
		{
			it->body = "";
			err_code = parsePlain(oss, *it);
		}
	}
	return (err_code);
}
