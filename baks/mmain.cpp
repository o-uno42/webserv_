// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   main.cpp                                           :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: aeid <aeid@student.42.fr>                  +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/01/29 12:56:38 by tjuvan            #+#    #+#             */
// <<<<<<< HEAD
// /*   Updated: 2025/02/04 21:34:54 by aeid             ###   ########.fr       */
// =======
// /*   Updated: 2025/02/03 16:48:37 by tjuvan           ###   ########.fr       */
// >>>>>>> rest
// /*                                                                            */
// /* ************************************************************************** */

// #include <cstddef>
// # define DEFAULT_PATH "default_path/default.conf"
// # include "../includes/Parser.hpp"

// std::string handle_input (int argc, char **argv) {
// 	if (argc == 1)
// 		return DEFAULT_PATH;
// 	if (argc == 2)
// 		return argv[1];
// 	if (argc > 2) {
// 		std::cerr << "Invalid number of arguments" << std::endl;
// 		exit (1);
// 	}
// 	return "";
// }

// // 🔹 Function to print all request struct fields
// void printRequestStruct(const t_request &req)
// {
// <<<<<<< HEAD
// 	std::string path = handle_input(argc, argv);
// 	std::vector<Server> servers_vector;
// 	try {
// 		if (argc == 2)
// 		{
// 			Parser parser(argv[1]);
// 			servers_vector = parser.getServers();
// 		}
// 		else
// 		{
// 			Parser parser(path);
// 			servers_vector = parser.getServers();
// 		}
// 	} catch (const Parser::FileExcep &e) {
// 		std::cerr << RED << e.what() << RESET << std::endl;
// 	}
// 	std::cout << "Server port: " <<  servers_vector[0].getPort() << std::endl;
// 	std::cout << "Server host: " <<  servers_vector[0].getHost() << std::endl;
// 	std::cout << "Server name: " <<  servers_vector[0].getServerName() << std::endl;
// 	const std::vector<std::string> &aliases = servers_vector[0].getServerAliases();
// 	if (!aliases.empty()) {
// 		std::cout << "Server aliases: ";
// 		for (size_t i = 0; i < aliases.size(); ++i) {
// 			std::cout << aliases[i];
// 			if (i < aliases.size() - 1) {
// 				std::cout << ", ";
// 			}
// 		}
// 		std::cout << std::endl;
// 	} else {
// 		std::cout << "Server aliases: None" << std::endl;
// 	}
// 	std::cout << "Error page: " <<  servers_vector[0].getErrorPage() << std::endl;
// 	std::cout << "Client max body size: " <<  servers_vector[0].getClientMax() << std::endl;
// 	std::cout << "Root: " <<  servers_vector[0].getRoot() << std::endl;
// 	std::cout << "Index: " <<  servers_vector[0].getIndex() << std::endl;
// 	for (size_t i = 0; i < servers_vector.size(); i++)
// 		startSocket(servers_vector[i]);
// 	//std::cout << path << std::endl
// 	return 0;
// }
// =======
//     std::cout << "📌 Parsed Request Data:\n";
//     std::cout << "Method: " << (req.method.empty() ? "[Not Provided]" : req.method) << "\n";
//     std::cout << "Path: " << (req.path.empty() ? "[Not Provided]" : req.path) << "\n";
//     std::cout << "Protocol: " << (req.protocol.empty() ? "[Not Provided]" : req.protocol) << "\n";
//     std::cout << "Host: " << (req.host.empty() ? "[Not Provided]" : req.host) << "\n";
//     std::cout << "User-Agent: " << (req.user_agent.empty() ? "[Not Provided]" : req.user_agent) << "\n";
//     std::cout << "Accept: " << (req.accept.empty() ? "[Not Provided]" : req.accept) << "\n";
//     std::cout << "Accept-Language: " << (req.accept_language.empty() ? "[Not Provided]" : req.accept_language) << "\n";
//     std::cout << "Connection: " << (req.connection.empty() ? "[Not Provided]" : req.connection) << "\n";
//     std::cout << "Content-Type: " << (req.content_type.empty() ? "[Not Provided]" : req.content_type) << "\n";
// if (!req.content_options.empty()) {
//     std::cout << "Content-Option: " << (req.content_options[0].empty() ? "[Not Provided]" : req.content_options[0]) << "\n";
// } else {
//     std::cout << "Content-Option: [Not Provided]\n";
// }
//     std::cout << "Body: " << (req.body.empty() ? "[Not Provided]" : req.body) << "\n";
//     std::cout << "line_break: " << req.line_break << "\n";
// }


// int main()
// {
//     // ✅ Valid HTTP request
//     std::string valid_request =
//         "GET /index.html HTTP/1.1\r\n"
//         "Host: example.com\r\n"
//         "User-Agent: TestClient/1.0\r\n"
//         "Accept: text/html\r\n"
//         "Connection: keep-alive\r\n"
//         "\r\n";  // Empty line separates headers from body

//     // ❌ Invalid HTTP request (missing method)
//     std::string invalid_request =
//         "/index.html HTTP/1.1\r\n"
//         "Host: example.com\r\n"
//         "User-Agent: TestClient/1.0\r\n"
//         "Accept: text/html\r\n"
//         "Connection: keep-alive\r\n"
//         "\r\n";

//     // ✅ POST request with a body (JSON data)
//     std::string post_request =
//         "POST /submit HTTP/1.1\r\n"
//         "Host: example.com\r\n"
//         "User-Agent: TestClient/1.0\r\n"
//         "Accept: application/json\r\n"
//         "Content-Type: application/json; charset=UTF-8\r\n"
//         "Content-Length: 42\r\n"
//         "Connection: keep-alive\r\n"
//         "\r\n"
//         "{ \"username\": \"test_user\", \"password\": \"1234\" }";  // Body starts after the empty line



//     // 🌟 Test valid request
//     std::cout << "=== Testing Valid Request ===\n";
//     t_request req_valid;
//     int status = parseRequest(valid_request, req_valid);
//     /* if (status == 100) */
//         printRequestStruct(req_valid);
//     /* else */
//     /*     std::cout << "Parsing failed with error code: " << status << "\n"; */
// 		std::cout << "\n" << status << std::endl;

//     std::cout << "\n";

//     // 🌟 Test invalid request
//     std::cout << "=== Testing Invalid Request ===\n";
//     t_request req_invalid;
//     status = parseRequest(invalid_request, req_invalid);
//     /* if (status == 100) */
//         printRequestStruct(req_invalid);
//     /* else */
//     /*     std::cout << "Parsing failed with error code: " << status << "\n"; */
// 		std::cout << "\n" << status << std::endl;

//     // 🌟 Test POST request with a body
//     std::cout << "=== Testing POST Request with Body ===\n";
//     t_request req_post;
//     status = parseRequest(post_request, req_post);
//     printRequestStruct(req_post);
//     std::cout << "\n" << status << std::endl;

//     return 0;
// }
// >>>>>>> rest
