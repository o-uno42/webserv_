#include "../../includes/Everything.hpp"

/** 
 * @description: save file sent by client
 * @params: request, server
 * @returns: the response string
 */
std::string saveFileHandler(t_request &request, Server &server){
    std::string filename = "";
    if (!request.body.empty()) {
        chmod(request.full_path.c_str(), 0777);
        if (!request.multi_part.empty()) {
            for (size_t i = 0; i < request.multi_part.size(); ++i) { //save files in database/files
                filename = extractFilename(request.multi_part[i]);
                if (!filename.empty()) {
                    if (!saveFileInDatabase(filename, request.full_path,
                            request.multi_part[i])) {
                        return ResponseMessage::sendErrorResponse(500, server);
                        // TODO Substitute 500 with actual error code
                    };
                }
            }
        }
    }
    // LOG_RED("FILE UPLOADED :" << filename);
    return setHTMLPage("File Uploaded!", "File uploaded: "
            + filename, request.path +  "/" + filename);
}

/** 
 * @description: save user in data.txt
 * @params: request, server e file path
 * @returns:response string
 */
std::string registrationHandler(t_request &request, Server &server, std::string file_path){
    request.content_type = "text/html"; //TODO IS this necessary?
    if (!request.body.empty()) {
        int file_fd = open(file_path.c_str(), O_RDWR | O_APPEND , 0666);
        if (file_fd == -1) {
            return ResponseMessage::sendErrorResponse(500, server); // TODO 500 ?
        }
        std::string entry = request.body + "\n";
        write(file_fd, entry.c_str(), entry.size()); //writing user in data.txt
        close(file_fd);
    }
    return setHTMLPage("Registration Successful",
            "Received data:\n" + request.body + " - - - You are now registered!",
                "/happy_shrek.jpg");
}

/** 
 * @description: checks if the user exists in the database
 * @params: the file to search in and the user to search
 * @returns: true/false
 */
 bool user_exists(const std::string &filename, const std::string &search, Server &server) {

    std::ifstream file(filename.c_str());
    if (!file){
        ResponseMessage::sendErrorResponse(500, server); //TODO Check if 500
        return false;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (!search.empty() && search.length() > 7){
            size_t pos = search.find('?');
            std::string user_to_search = search.substr(pos + 1);
            if (line == user_to_search) {
                file.close();
                return true;
            }
        }
        else
            // LOG_RED("- - - No user provided"); // TODO rmv this print
        if (line == search) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

/** 
*@description: simple login handler
*@params: request and server
*@called by: getContent
*@returns: the right html page
*/
std::string loginHandler(t_request &request, Server &server){
    if (user_exists(request.full_path, request.path, server)) {
        return setHTMLPage("Welcome back!",
                "This is your swamp.", "/swamp.jpg");
    }
    return setHTMLPage("Ops! You aren't registered yet, are you?",
            "This could become your swamp!", "/ciuchino.jpg");
}

/** 
*@description: checks if the file is an image
*@params: the filename
*@called by: setHTMLPage
*@returns: true/false
*/
bool isFileImage(std::string filename) {
    std::string extension = filename.substr(filename.find_last_of('.') + 1);
    if (extension == "jpg" || extension == "jpeg" ||
            extension == "png" || extension == "gif" ||
            extension == "JPG" || extension == "JPEG" ||
            extension == "PNG" || extension == "GIF") {
        return true;
    }
    return false;
}

/** 
*@description: creates the html page for the login
*@params: title, the message and the url of background
*@called by: getContent
*@returns: the html page as string
*/
std::string setHTMLPage(std::string title, std::string msg, std::string url_bg){
    std::string bg = "/happy_shrek.jpg"; //default background
    if (isFileImage(url_bg)) {
        bg = url_bg;
    }
    return "<html><head>"
                "<style>"
                "body {"
                "    background-image: url('" + bg + "');"
                "    background-size: cover;"
                "    background-position: center;"
                "    background-repeat: repeat;"
                "}"
                "</style>"
                "</head><body>"
                "<h1>"+ title + "</h1>"
                "<h2>" + msg + "</h2>"
                "</body></html>";
}

/** 
*@description: extract filename from request
*@params: Multi_part
*@called by: getContent
*@returns: the filename (string)
*/
std::string extractFilename(const t_multi_part &part) {
        std::string filename = "";
        // extract  filename from disposition_params
        for (size_t i = 0; i < part.disposition_params.size(); ++i) {
            if (part.disposition_params[i].find("filename=") != std::string::npos) {
                filename = part.disposition_params[i].substr(9); // remove 'filename="' (9 chars)
                filename = filename.substr(1, filename.size() - 3); //remove trailing '"'
                break;
            }
        }
        return filename;
}

/** 
*@description: saves the file uploaded in the database
*@params: the filename, the path and multi_part
*@called by: getContent
*@returns: true or false
*/
bool    saveFileInDatabase(std::string filename, std::string path, const t_multi_part &part) {
    if (!filename.empty()) {
        std::string dir;
        if (path.substr(path.size()) != "/")
            dir = path + "/" + filename;
        else
            dir = path + filename;
        FILE *file = fopen(dir.c_str(), "wb"); // wb -> binary write mode
        if (file) {
            fwrite(part.body.c_str(), 1, part.body.size(), file);
            fclose(file);
            LOG_MAGENTA("File saved: " << filename);
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}
