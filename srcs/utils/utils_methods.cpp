#include "../../includes/Everything.hpp"

/** 
*@description: Get handler
*@params: request, server and filepath of the file to get
*@called by: getContent
*@returns: response string
*/
std::string getHandler(t_request &request, Server &server, std::string file_path){
    std::string login_path = "";
    size_t pos = request.path.find('?');
    if (pos != std::string::npos) {
        login_path = request.path.substr(0, pos); // controllo il path fino a ? (inizio query)
    }
    if (login_path == "/database/data.txt") {
        return loginHandler(request, server);
    }
    std::ifstream file(file_path.c_str(), std::ios::binary);
    if (file) {
        return readFileContents(file);
    }
    return ResponseMessage::sendErrorResponse(404, server);
}

/** 
*@description: Post handler
*@params: request, server and filepath of the file to get
*@called by: getContent
*@returns: response string
*/
std::string postHandler(t_request &request, Server &server, std::string file_path) {
    if (request.path == "/database/data.txt") {
        return registrationHandler(request, server, file_path);
    }
    else if (request.path == "/database/files/") { 
        return saveFileHandler(request, server);
    }
    return setHTMLPage("POST received",
            "Received data:\n" + request.body, "/happy_shrek.jpg");
}

/** 
*@description: Delete handler
*@params: request, server and filepath of the file to get
*@called by: getContent
*@returns: response string
*/
std::string deleteHandler(t_request &request, Server &server, std::string file_path) {
    size_t pos = request.body.find('=');
            std::string file_to_remove =  request.body.substr(pos + 1);
    std::string full_file_path = file_path + "/" + file_to_remove;
    if (access(full_file_path.c_str(), F_OK) != 0) {
        request.response_code = 404;
        return ResponseMessage::sendErrorResponse(404, server);
        // return (setHTMLPage("File to remove not found" + ResponseMessage::sendErrorResponse(404, server), "hi", 0));
    }
    if (!remove(full_file_path.c_str())) {
        return setHTMLPage("Resource Deleted!", "The requested resource has been deleted.", "/happy_shrek.jpg");
    }
    request.response_code = 500;
    return ResponseMessage::sendErrorResponse(500, server);
}
