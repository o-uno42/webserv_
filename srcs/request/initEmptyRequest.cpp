/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initEmptyRequest.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thiew <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 14:25:22 by thiew             #+#    #+#             */
/*   Updated: 2025/05/09 14:30:28 by thiew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/request.hpp"
# include "../../includes/Server.hpp"


int initEmptyRequest(t_request &req, int response_code)
{
	req.method = "undefined";
	req.path = "undefined";
	req.parsed_path = "undefined";
	req.full_path = "undefined";
	req.path_query = "undefined";
	req.path_type = "undefined";
	req.return_code = -1;
	req.return_string = "undefined";
	req.protocol = "undefined";
	req.host = "undefined";
	req.user_agent = "undefined";
	req.accept = "undefined";
	req.accept_language = "undefined";
	req.connection = "undefined";
	req.cookies = "undefined";
	req.cookies_vector.clear();
	req.content_type = "undefined";
	req.content_options.clear();
	req.content_length = 0;
	req.response_code = response_code;
	req.body = "undefined";
	req.line_break = 0;
	req.multi_part.clear();
	req.encoded_params.clear();
	req.params.clear();
	req.methods.clear();
	req.header_len = 0;
	return (100);
}
