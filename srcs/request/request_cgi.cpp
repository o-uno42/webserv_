#include "../../includes/Everything.hpp"
/** 
 * @description: checks if a request is for CGI
 * @params: the request
 * @returns: true if its for cgi, otherwise false
 */
 bool isCgiRequest(const t_request &request) {
    // method 1: is it a CGI extension?
    std::string path = request.path;
    size_t dotPos = path.find_last_of('.');
    if (dotPos != std::string::npos) {
        std::string extension = path.substr(dotPos);
        if (extension == ".sh" || extension == ".py" || extension == ".php" || extension == ".js") {
            return true;
        }
    }
    // 2 the path specifics cgi-bin
    if (path.find("/cgi-bin/") != std::string::npos) {
        return true;
    }
    return false;
}
