//BACKUP IS REQUEST COMPLETE
//  bool isRequestComplete(const std::string &request) { 
//     // find the header end
//     size_t headerEnd = request.find("\r\n\r\n");
//     if (headerEnd == std::string::npos) {
//         headerEnd = request.find("\n\n"); //for mixed line endings
//     }
//     if (headerEnd == std::string::npos) {
//         return false; //the request is not complete
//     } 

//     std::string headers = request.substr(0, headerEnd);  ;

//     // Manual search for Content-Length
//     size_t contentLengthPos = headers.find("Content-Length:");
//     if (contentLengthPos != std::string::npos) { 
//         LOG_RED("Content-Length header found at position: " << contentLengthPos);
        
//         // Manual parsing of Content-Length value
//         size_t valueStart = contentLengthPos + 15;
//         while (valueStart < headers.size() && (headers[valueStart] == ' ' || headers[valueStart] == '\t')) 
//             ++valueStart;
        
//         // Manually find the end of the line
//         size_t valueEnd = valueStart;
//         while (valueEnd < headers.size()) {
//             if (headers[valueEnd] == '\r' || headers[valueEnd] == '\n') {
//                 break;
//             }
//             ++valueEnd;
//         }
        
//         if (valueEnd == valueStart) {
//             LOG_RED("Could not find Content-Length value");
//             return false;
//         }
        
//         // extract content-length value
//         std::string contentLengthStr = headers.substr(valueStart, valueEnd - valueStart);
//         LOG_RED("Content-Length string: " << contentLengthStr);
        
//         // convert to int
//         int contentLength = 0;
//         try {
//             contentLength = atoi(contentLengthStr.c_str());
//         } catch (...) {
//             LOG_RED("Failed to convert Content-Length to integer");
//             return false;
//         }
//         // calculate full request size 
//         size_t bodyStart = headerEnd + 4; // for "\r\n\r\n"
//         if (bodyStart > request.size())
//             bodyStart = headerEnd + 2; // for "\n\n"
//         if (request.size() >= bodyStart + contentLength) { 
//             return true; 
//         }
//         return false; 
//     }
// 	//no content length header, so the request is complete
//     return true; 
// }

// BACKUP MENU PRINTS
	// std::cout << "Server port: " <<  servers_vector[0].getPorts() << std::endl;
	// const std::vector<int> &_ports = servers_vector[0].getPorts();
	// if (!_ports.empty()) {
	// 	std::cout << "Server port: ";
	// 	for (size_t i = 0; i < _ports.size(); ++i) {
	// 		std::cout << _ports[i];
	// 		if (i < _ports.size() - 1) {
	// 			std::cout << ", ";
	// 		}
	// 	}
	// 	std::cout << std::endl;
	// } else {
	// 	std::cout << "Server ports: None" << std::endl;
	// }
	// std::cout << "Server host: " <<  servers_vector[0].getHost() << std::endl;
	// std::cout << "Server name: " <<  servers_vector[0].getServerName() << std::endl;
	// std::cout << "Server root: " <<  servers_vector[0].getRoot() << std::endl;
	// const std::vector<std::string> &aliases = servers_vector[0].getServerAliases();
	// if (!aliases.empty()) {
	// 	std::cout << "Server aliases: ";
	// 	for (size_t i = 0; i < aliases.size(); ++i) {
	// 		std::cout << aliases[i];
	// 		if (i < aliases.size() - 1) {
	// 			std::cout << ", ";
	// 		}
	// 	}
	// 	std::cout << std::endl;
	// } else {
	// 	std::cout << "Server aliases: None" << std::endl;
	// }
	// std::cout << "Error page: " <<  servers_vector[0].getErrorPage() << std::endl;
	// std::cout << "Client max body size: " <<  servers_vector[0].getClientMax() << std::endl;
	// std::cout << "Root: " <<  servers_vector[0].getRoot() << std::endl;
	// std::cout << "Index: " <<  servers_vector[0].getIndex() << std::endl;
	// std::cout << "\n\n\n\n";
