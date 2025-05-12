/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestErrorMappings.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjuvan <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:55:01 by tjuvan            #+#    #+#             */
/*   Updated: 2025/01/30 16:11:08 by tjuvan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ResponseMessage.hpp"

std::map<int, std::string>	ResponseMessage::initErrorsShort()
{
	std::map<int, std::string>	short_message;

    // 1xx Informational
    short_message[100] = "Continue";
    short_message[101] = "Switching Protocols";
    short_message[102] = "Processing";

    // 2xx Success
    short_message[200] = "OK";
    short_message[201] = "Created";
    short_message[202] = "Accepted";
    short_message[204] = "No Content";
    short_message[206] = "Partial Content";

    // 3xx Redirection
    short_message[300] = "Multiple Choices";
    short_message[301] = "Moved Permanently";
    short_message[302] = "Found";
    short_message[304] = "Not Modified";
    short_message[307] = "Temporary Redirect";
    short_message[308] = "Permanent Redirect";

    // 4xx Client Errors
    short_message[400] = "Bad Request";
    short_message[401] = "Unauthorized";
    short_message[403] = "Forbidden";
    short_message[404] = "Not Found";
    short_message[405] = "Method Not Allowed";
    short_message[408] = "Request Timeout";
    short_message[409] = "Conflict";
    short_message[410] = "Gone";
    short_message[411] = "Length Required";
    short_message[413] = "Payload Too Large";
    short_message[414] = "URI Too Long";
    short_message[415] = "Unsupported Media Type";
    short_message[429] = "Too Many Requests";
    
    // 5xx Server Errors
    short_message[500] = "Internal Server Error";

    return short_message;
}

std::map<int, std::string>	ResponseMessage::initErrorsLong()
{
    std::map<int, std::string> long_message;

    // 1xx Informational
    long_message[100] = "The server has received the request headers, and the client should proceed to send the request body.";
    long_message[101] = "The requester has asked the server to switch protocols and the server has agreed to do so.";
    long_message[102] = "The server has received and is processing the request, but no response is available yet.";

    // 2xx Success
    long_message[200] = "The request was successful and the server returned the requested resource.";
    long_message[201] = "The request was successful and a new resource was created.";
    long_message[202] = "The request has been accepted for processing, but the processing has not been completed.";
    long_message[204] = "The server successfully processed the request, but is not returning any content.";
    long_message[206] = "The server is delivering only part of the resource due to a range header sent by the client.";

    // 3xx Redirection
    long_message[300] = "The request has more than one possible response. The user-agent or user should choose one of them.";
    long_message[301] = "The URL of the requested resource has been changed permanently. The new URL is given in the response.";
    long_message[302] = "The requested resource resides temporarily under a different URL.";
    long_message[304] = "The resource has not been modified since the version specified by the request headers.";
    long_message[307] = "The requested resource resides temporarily under a different URL, and the client should use the original URL for future requests.";
    long_message[308] = "The requested resource has been moved permanently to a new URL, and the client should use the new URL for future requests.";

    // 4xx Client Errors
    long_message[400] = "The server could not understand the request due to invalid syntax.";
    long_message[401] = "The client must authenticate itself to get the requested response.";
    long_message[403] = "The client does not have access rights to the content.";
    long_message[404] = "The server can not find the requested resource.";
    long_message[405] = "The request method is known by the server but is not supported by the target resource.";
    long_message[408] = "The server would like to shut down this unused connection.";
    long_message[409] = "The request could not be completed due to a conflict with the current state of the target resource.";
    long_message[410] = "The requested resource is no longer available and will not be available again.";
    long_message[411] = "The server refuses to accept the request without a defined Content-Length header.";
    long_message[413] = "The request is larger than the server is willing or able to process.";
    long_message[414] = "The URI requested by the client is longer than the server is willing to interpret.";
    long_message[415] = "The media format of the requested data is not supported by the server.";
    long_message[429] = "The user has sent too many requests in a given amount of time.";

    return long_message;
}
