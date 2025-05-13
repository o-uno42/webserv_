#include "../includes/Everything.hpp"

volatile sig_atomic_t active = true;

/** 
 * @description: this handles the signal cntrl c to quit the server
 */
 void handleSignal(int sig){
    (void)sig;
	LOG_BLUE("- - - - - - - You pressed Ctrl+C. Closing WebShrerver. - - - - - - -");
    active = false;
}

/** 
 * @description: checks if an HTTP request is complete
 * @params: buffer containing the request data
 * @returns: true or false
 */
 bool isRequestComplete(t_request &reqStruct, const std::string &request) {
    size_t headerEnd = request.find("\r\n\r\n"); // find end of headers
    if (headerEnd == std::string::npos) {
        return false; // headers not completely received yet
    }
	reqStruct.header_len = headerEnd + 4;

    std::string headers = request.substr(0, headerEnd);  // extract headers

    size_t contentLengthPos = headers.find("Content-Length:"); // check for Content-Length
    if (contentLengthPos != std::string::npos) {
        // calculate content len value
        size_t start = contentLengthPos + 15; // skipping "Content-Length:" (15 char)
        while (start < headers.size() && headers[start] == ' ')
            ++start;
        size_t end = headers.find("\r\n", start);
        if (end == std::string::npos) return false;
        int contentLength = atoi(headers.substr(start, end - start).c_str());
        
        // calculate full request size
        size_t bodyStart = headerEnd + 4; // +4 due to "\r\n\r\n"
        if (request.size() >= bodyStart + contentLength) {
            return true;
        }
        return false;
    }
    // if there's no Content-Length, assume request is complete, like its a GET request (which has no Content lenght)
    return true;
}

/** 
*@description: Here I create the server socket
*@params: The server
*@called by: The main (for now)
*@returns: Int 0 or 1
*/
int    Socket::startServerSocket(std::vector<Server> servers)
{
	std::vector<Socket> serverSockets(servers.size());
	EpollHandler 				epollHandler;
	t_request 					request;
	t_response					resStruct;
	std::vector<int>		 client_fds;

	resStruct.connection = "";
	resStruct.server = "";
	resStruct.content_type = "";
	resStruct.location = std::vector<Location>();

	signal(SIGINT, handleSignal);
	initEmptyRequest(request, -1);

	if(!checkDoublePort(servers)){return 1;};

	LOG_GREEN( "- - - Number of servers : " << servers.size());
	for (size_t i = 0; i < servers.size(); i++) {
		if(!serverSockets[i].launchingServer(servers[i])){};
	}

	LOG_GREEN("\n- - - Finished launching servers\n");

	if(!initEpollWithServers(epollHandler, serverSockets, servers)){return 1;};

	// its a map of client_fd and vector of char, so each client has its own buffer
	std::map<int, std::string> _read_buffer;
	std::map<int, std::string> _write_buffer;
	std::map<int, int> clientToServerIndex;

	while (active) {

		int num_events = epollHandler.waitEpoll(10000);
		if (num_events < 0) {
			if (!active)
				continue;
			// return 1;
		}
		
		for (int i = 0; i < num_events; i++) {
			int current_fd = epollHandler.getEpollEvents()[i].data.fd;
			bool isServerSocket = false;
			int serverIndex = -1;

			//check if current fd is a server socket (and setting serverIndex)
			for (size_t j = 0; j < serverSockets.size(); j++) {
				if (std::find(serverSockets[j]._socket_fds.begin(), 
					serverSockets[j]._socket_fds.end(), current_fd) != serverSockets[j]._socket_fds.end()) {
					isServerSocket = true;
					serverIndex = j;
					break;
				}
			}
			
			// check if this is a server socket (for new connections)
			if (isServerSocket) {
				manageServerSocket(i, serverIndex, epollHandler, current_fd, clientToServerIndex, client_fds);
			}
			// if its instead a client socket event
			else {
				// get the server index this client is connected to
				int serverIndex = clientToServerIndex[current_fd];
				// check the type of the event
				if (epollHandler.getEpollEvents()[i].events & EPOLLIN) {
					
					LOG_YELLOW(DIM << "- - - Reading data from client FD [" << current_fd << "]");
					
					// read data from client
					std::string &request_buf = _read_buffer[current_fd];
					char tmp_read[BUFFER_SIZE];
					int valread = recv(current_fd, tmp_read, BUFFER_SIZE, 0);
					
					if (valread > 0) {
						LOG_CYAN("\n+ + + Processing request");
						request_buf.append(tmp_read, valread); // append data to the buffer
						// process the request
						if (isRequestComplete(request, request_buf)) {
							// LOG_CYAN("Body size: " << request_buf.size() - request.header_len);
							if (request_buf.size() - request.header_len > BODY_LIMIT) {
								initEmptyRequest(request, 413);
								LOG_RED("Request exceeded BODY_LIMIT, from client FD [" << current_fd <<  "]" );
								std::string content = ResponseMessage::sendErrorResponse(413, servers[serverIndex]);
								std::string response = setHeaderResponse(request, 413, content, servers[serverIndex]);
								epollHandler.modifySocketFromEpoll(current_fd, EPOLLOUT); // now it has to switch to write mode
								_write_buffer[current_fd] = response; // and store the response for sending it later
							} else {
								LOG_CYAN("Request received from client FD [" << current_fd  << "]");
								// parsing request
								t_request request; 
								int code = parseRequest(request_buf, request, servers[serverIndex]);
								LOG_CYAN("+ + + parseRequest returned status code: " << code);
								
								if (code == 100) {
									printRequestClear(request);
									resStruct.connection = request.connection;
									resStruct.server = servers[serverIndex].getServerName();
									resStruct.content_type = request.content_type;
									resStruct.location = servers[serverIndex].getLocations();

									if (isCgiRequest(request)) {
										std::string content = cgiHandler(servers[serverIndex], request);
										std::string response = cgiSetHeaderResponse(request, request.response_code, content, servers[serverIndex]);//setResponse(resStruct, request, cgi_output, servers[serverIndex]);
										epollHandler.modifySocketFromEpoll(current_fd, EPOLLOUT);
										_write_buffer[current_fd] = response;
									} else {
										// create content based on the request
										std::string content = getContent(request, servers[serverIndex]);
										std::string response = setHeaderResponse(request, request.response_code, content, servers[serverIndex]);//setResponse(resStruct, request, content, servers[serverIndex]);
										epollHandler.modifySocketFromEpoll(current_fd, EPOLLOUT);
										_write_buffer[current_fd] = response;
									}
								} else {
									std::string content = ResponseMessage::sendErrorResponse(code, servers[serverIndex]);;
									std::string response = setHeaderResponse(request, request.response_code, content, servers[serverIndex]);
									epollHandler.modifySocketFromEpoll(current_fd, EPOLLOUT); //  change the fd to write mode
									_write_buffer[current_fd] = response; // and store the response for sending it later
								}
							}
							request_buf.clear(); // clear the buffer
						}
					}
					else if (valread == 0) { //the client closed the connection
						LOG_GREEN("- - - Client FD [" << current_fd << "] disconnected.");
						epollHandler.removeSocketFromEpoll(current_fd);
						closeFdAndClearBuffer(current_fd, _read_buffer, clientToServerIndex);
					}
					else { // recv failed
						// LOG_RED("- ! - Recv failed");
						Error::runningError("error: recv failed");
						closeFdAndClearBuffer(current_fd, _read_buffer, clientToServerIndex);
					}
				}
				else if (epollHandler.getEpollEvents()[i].events & EPOLLOUT) {
					sendResponse(_write_buffer, current_fd, epollHandler);
					epollHandler.modifySocketFromEpoll(current_fd, EPOLLIN);
				}
			}
		}
	}
	cleanUp(serverSockets, client_fds, epollHandler, servers);
	return 0;
}
