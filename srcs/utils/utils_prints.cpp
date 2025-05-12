#include "../../includes/Everything.hpp"

void printMultiPart(const std::vector<t_multi_part> &multi_part) {

    std::cout << CYAN << "Multi part size : "<< multi_part.size() << RESET << std::endl;
    for (size_t i = 0; i < multi_part.size(); ++i) {
        std::cout << CYAN << "Part " << i + 1 << ":" << std::endl;

        // Print disposition parameters (e.g., name, filename)
        std::cout << "  Disposition Parameters: ";
        for (size_t j = 0; j < multi_part[i].disposition_params.size(); ++j) {
            std::cout << multi_part[i].disposition_params[j] << " ";
        }
        std::cout << std::endl;

        // Print Content-Type
        // std::cout << "  Content-Type: " << multi_part[i].content_type << std::endl;

        // Print Encoded Parameters
        std::cout << "  Encoded Parameters: ";
        for (size_t j = 0; j < multi_part[i].encoded_params.size(); ++j) {
            std::cout << multi_part[i].encoded_params[j] << " ";
        }
        std::cout << std::endl;

        // Print Body Size (Don't print raw body if it's binary)
        std::cout << "  Body Size: " << multi_part[i].body.size() << " bytes" << std::endl;

        std::cout << "---------------------------------" << RESET << std::endl;
    }
}


void printRequestClear(const t_request &req){
	LOG_CYAN(DIM << "\n+ + + + + Request + + + + +");
    if(!req.method.empty())
	    LOG_CYAN("Method                      : " <<  req.method);
    if (!req.path.empty())
        LOG_CYAN("Path                        : " << req.path);
    if (!req.full_path.empty())
	    LOG_CYAN("Path full                   : " << req.full_path);
    if (!req.path_query.empty())
	LOG_CYAN("Query                       : " << req.path_query);
    if (!req.path_type.empty())
	    LOG_CYAN("Path type                   : " << req.path_type);
    if (!req.protocol.empty())
	    LOG_CYAN("Protocol                    : " << req.protocol);
    if (!req.host.empty())
	    LOG_CYAN("Host                        : " << req.host);
    if (!req.user_agent.empty())
	    LOG_CYAN(DIM << "User-Agent                  : " << req.user_agent);
    if (!req.accept.empty())
	    LOG_CYAN(DIM << "Accept                      : " << req.accept);
    if (!req.accept_language.empty())
	    LOG_CYAN(DIM << "Accept-Language             : " << req.accept_language);
    if (!req.connection.empty())
	    LOG_CYAN(DIM << "Connection                  : " << req.connection);
	if (!req.cookies.empty())
		LOG_CYAN(DIM << "Cookies                     : " << req.cookies);
	if (!req.cookies_vector.empty()) {
		for (size_t i = 0; i < req.cookies_vector.size(); i++)
			LOG_CYAN(DIM << "Cookies-vector              : " << req.cookies_vector[i]);
	}
	if (!req.content_type.empty())
		LOG_CYAN(DIM << "Content-Type                : " << req.content_type);
	if (!req.content_options.empty()){
		LOG_CYAN(DIM << "Content-Option              : " << (req.content_options[0].empty() ? "[Not Provided]" : req.content_options[0]));
	}
    if (!req.body.empty()){
        LOG_CYAN(DIM << "Body                        : " << req.body.substr(0, 150) << "...");
        LOG_CYAN(DIM << "{ ! Not printing full request body to readability reasons }\n");
    }
    if (!req.encoded_params.empty())
    {
        std::cout << CYAN << DIM<< "Encoded params              : ";
        for (size_t j = 0; j < req.encoded_params.size(); ++j)
        {
            std::cout << req.encoded_params[j];
            if (j + 1 < req.encoded_params.size())
                std::cout << CYAN << DIM << ", ";
        }
        std::cout << RESET << std::endl;
    }
    LOG_CYAN(DIM << "Line_break                  : " << req.line_break);
    LOG_CYAN(DIM << "+ + + + + + + + + + + + + +\n");
}

void printRequest(const t_request &req){
	LOG_CYAN("\n+ + + + + Request + + + + +");
	LOG_CYAN("Method                      : " << (req.method.empty() ? "[Not Provided]" : req.method));
    LOG_CYAN("Path                        : " << (req.path.empty() ? "[Not Provided]" : req.path));
	LOG_CYAN("Path full                   : " << (req.full_path.empty() ? "[Not Provided]" : req.full_path));
	LOG_CYAN("Query                       : " << (req.path_query.empty() ? "[Not Provided]" : req.path_query));
	LOG_CYAN("Path type                   : " << (req.path_type.empty() ? "[Not Provided]" : req.path_type));
	LOG_CYAN("Protocol                    : " << (req.protocol.empty() ? "[Not Provided]" : req.protocol));
	LOG_CYAN("Host                        : " << (req.host.empty() ? "[Not Provided]" : req.host));
	LOG_CYAN("User-Agent                  : " << (req.user_agent.empty() ? "[Not Provided]" : req.user_agent));
	LOG_CYAN("Accept                      : " << (req.accept.empty() ? "[Not Provided]" : req.accept));
	LOG_CYAN("Accept-Language             : " << (req.accept_language.empty() ? "[Not Provided]" : req.accept_language));
	LOG_CYAN("Connection                  : " << (req.connection.empty() ? "[Not Provided]" : req.connection));
	LOG_CYAN("Content-Type                : " << (req.content_type.empty() ? "[Not Provided]" : req.content_type));
	LOG_CYAN("Cookies                     : " << (req.cookies.empty() ? "[Not Provided]" : req.cookies));
	if (!req.content_options.empty()){
		LOG_CYAN("Content-Option              : " << (req.content_options[0].empty() ? "[Not Provided]" : req.content_options[0]));
	}
	else {
		LOG_CYAN("Content-Option              : [Not Provided]");
	}
	LOG_CYAN("Body                        : " << (req.body.empty() ? "[Not Provided]" : req.body));
	std::cout << CYAN << "Encoded params              : ";
	if (req.encoded_params.empty()) {
		std::cout << CYAN << "[Empty]" << RESET << std::endl;
	} else {
		for (size_t j = 0; j < req.encoded_params.size(); ++j)
		{
			std::cout << req.encoded_params[j];
			if (j + 1 < req.encoded_params.size())
				std::cout << CYAN << ", ";
		}
		std::cout << RESET << std::endl;
	}
    LOG_CYAN("Line_break                  : " << req.line_break);
    LOG_CYAN("+ + + + + + + + + + + + + +\n");
}


