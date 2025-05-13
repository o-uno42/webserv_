#include "../../includes/Everything.hpp"
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/** 
 * @description: send to client the response
 * @params: the socket fd and the message
 * @called by: sendResponse
 * @returns: the response
 */
bool sendToClient(int fd, std::string message, EpollHandler &epollHandler) {
    if (send(fd, message.c_str(), message.size(), 0) < 0)  {
        Error::runningError("error: send failed");
        epollHandler.removeSocketFromEpoll(fd);
        close(fd);
        return false;
    }
    return true;
}

/** 
 * @description: set the header response for CGI
 * @params: request, code, content and server
 * @returns: the response
 */
std::string cgiSetHeaderResponse(t_request &req, const int &code, const std::string &content, Server &server){
    (void)server;
    int correct_code = (code ? code : req.response_code);
    if (correct_code == 100) {
        if (req.method == "GET" || req.method == "DELETE")
            correct_code = 200;
        else if (req.method == "POST")
            correct_code = 201;
        else
            correct_code = 405;
    }
    size_t header_end = content.find("\r\n\r\n");
    std::string headers, body;
    if (header_end != std::string::npos) {
        headers = content.substr(0, header_end);
        body = content.substr(header_end + 4);
    } else {
        headers = "Content-Type: text/plain";
        body = content;
    }
    std::ostringstream response;
    // response << "HTTP/1.1 " << correct_code << " " << ResponseMessage::getShortMessage(correct_code) << "\r\n";
    // cgi headers
    response << headers << "\r\n";
    // if (headers.find("Content-Length:") == std::string::npos) {
    //     response << "Content-Length: " << body.size() << "\r\n";
    // }
    response << "\r\n" << body;
    return response.str();
}

/** 
 * @description: set the header response
 * @params: response, code, content and server
 * @returns: the response
 */
std::string setHeaderResponse(t_request &req, const int &code, const std::string &content, Server &server) {

    (void)server;
    int correct_code = 405;
    if (code)
        correct_code = code;
    else
        correct_code = req.response_code;
    if (req.return_code != -1){
        correct_code = req.return_code;
    }
    if (correct_code == 100){
        if (req.method == "GET" || req.method == "DELETE") {
            correct_code = 200;
        } else if (req.method == "POST") {
            correct_code = 201;
        } else {
            correct_code = 405;
        }
    }
    std::stringstream status_code;
	std::stringstream len;
	status_code << correct_code;
	std::string status_code_str = status_code.str();

    std::string response = "HTTP/1.1 " + status_code_str +  " " + ResponseMessage::getShortMessage(correct_code) + "\r\n"
                                    + "Content-Type: text/html\r\n"
                                    + "Content-Length: " + itoa(content.length()) + "\r\n"
                                    + "Connection: " + (req.connection.empty() ? "keep-alive" : req.connection) + "\r\n"
                                    + "Location: " + req.return_string + "\r\n\r\n"
                                    + content;
    return response;
}

/** 
 * @description: create an HTTP response based on the request
 * @params: the request struct and the content
 * @returns: the response
 */
//  std::string setResponse(t_response &res, t_request &req, const int &code, const std::string &content, Server &server) {
//     // if the content is already a CGI response we need to return it as is
//     // bool is_cgi_response = (content.find("HTTP/1.1") == 0 || 
//     // content.find("Status:") == 0 ||
//     // content.find("Content-Type:") != std::string::npos);
//     // if (is_cgi_response) {
//     // return content;
//     // }
//     (void)server;
//     (void)res;
//     (void)req;

//     std::stringstream status_code;
// 	std::stringstream len;
// 	status_code << code;
// 	std::string status_code_str = status_code.str();
// 	// std::string body;

//     // std::string printableResponse;
//     // std::string redirection = dirIndexCheck(req, server, req.response_code);
//     // std::string status_code = "";
//     // if (req.response_code != 100){
//     //     status_code = itoa(req.response_code) + " " + ResponseMessage::getShortMessage(req.response_code);
//     // }
//     // else{
//     //     if (req.method == "GET" || req.method == "DELETE") {
//     //         status_code = "200 OK";
//     //     } else if (req.method == "POST") {
//     //         status_code = "201 Created";
//     //     } else {
//     //         status_code = "405 Method Not Allowed";
//     //     }
//     // }
//     // TODO handle redirection (3xx); Set to ResponseMessage[shortmessage]

//     LOG_CYAN( "TYPE : " << res.content_type);
//     int content_length = content.length();
//     std::string response = "HTTP/1.1 " + status_code_str +  + "\r\n";
//     // if (res.redirect)
//     //     response += "Location: " +...;
//     response += "Content-Type: text/html\r\n";
//     //if its not an HTML page, instead of test/html we should send application/octet-stream, but its complex to check propely
//     if (!res.server.empty())
//         response += "Server: " + (res.server) + "\r\n";
//     if (content_length >= 0)
//         response += "Content-Length: " + itoa(content_length) + "\r\n";
//     response += "Connection: " + (res.connection.empty() ? "keep-alive" : res.connection) + "\r\n";
//     //if connection is not specified by the request, it's set to keep-alive, as default for HTTP/1.1
//     response += "\r\n";
//     response += content;
    
//     return response;
// }

/** 
 * @description: generates or retrieves content based on the request
 * @params: request and server
 * @returns: the content
 */
 std::string getContent(t_request &request, Server &server) {
    std::string root_dir = server.getRoot();
    std::string path = request.path.empty() || request.path == "/" ? "/form.html" : request.path;
    std::string file_path = root_dir + path;

    if (request.path_type == "dir" && request.method == "GET") {
        return dirIndexCheck(request, server, request.response_code);
        // return autoIndexGenerator(request, request.response_code);
    }
    if (request.method == "GET") {
        return getHandler(request, server, file_path);
    }
    else if (request.method == "POST") {
        std::string debug = postHandler(request, server, file_path);
        // LOG_YELLOW(debug);
        return debug;
    }
    else if (request.method == "DELETE") {
        // LOG_RED("- - - - - - - - DELETE request");
        // LOG_YELLOW(deleteHandler(request, server, file_path));
        std::string debug = deleteHandler(request, server, file_path);
        // LOG_YELLOW(debug);
        return debug;
    }
    else{
        return ResponseMessage::sendErrorResponse(501, server); //method not allowed
    }
    return ResponseMessage::sendErrorResponse(405, server); //method not allowed
}


/** 
 * @description: here i send the response
 * @params: the write buffer, the current fd and the epoll handler
 * @returns: nothing but sends the response to the client
 */
void sendResponse(std::map<int, std::string> &_write_buffer, int client_fd, EpollHandler &epollHandler) {
    //get the response from the previous write buffer
    std::string &response_buf = _write_buffer[client_fd];
    if (!response_buf.empty()) {
        LOG_MAGENTA("\n+ + + Sending response");
        LOG_MAGENTA(response_buf.substr(0, 150) << "...");
        LOG_MAGENTA("{ ! Not printing full response to readability reasons }\n");
        sendToClient(client_fd, response_buf, epollHandler);
    }
    else {
        // if no data to send switch  to read mode
        epollHandler.modifySocketFromEpoll(client_fd, EPOLLIN);
    }
}
