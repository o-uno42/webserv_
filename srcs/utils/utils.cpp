#include "../../includes/Everything.hpp"

/** 
 * @description: reads the entire contents of a file
 * @params: A file
 * @returns: file contents as a string
 */
 std::string readFileContents(std::ifstream& file) {
    std::string content;
    // to get file size
    file.seekg(0, std::ios::end); //seekg moves the reading pointer to the end of the file
    std::streampos file_size = file.tellg(); //tellg returns the position of the reading pointer
    // (in this case the end of the file, so its size)
    file.seekg(0, std::ios::beg); //here the pointer goes back to the beginning
    //reserve space in the string
    content.reserve(file_size);
    // storing file content in a string
    char buffer[4096];
    while (file.read(buffer, sizeof(buffer))) {
        content.append(buffer, file.gcount()); //gcount returns the number of characters read
    }
    content.append(buffer, file.gcount());
    return content;
}

/** 
*@description: close the fd and clear the buffer
*@params: the fd and the buffer
*@called by: startSocket
*@returns: nothing
*/
void closeFdAndClearBuffer(int fd, std::map<int, std::string> buffer, std::map<int, int> clientToServerIndex) {
	close(fd);
	buffer.erase(fd);
	clientToServerIndex.erase(fd);
}

/** 
 * @description: converts an integer to a string (itoa)
 * @params: the num
 * @returns: string
 */
 std::string itoa(size_t num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}

int	checkValidPath(t_request &req, std::string &full_path)
{
	std::ifstream	file;
	// checking if path is file or dir -- 404 or 403 codes
	DIR *dir = opendir(full_path.c_str());
	if (dir && !req.path.empty() && req.path[req.path.size() - 1] == '/')
		req.path_type = "dir";
	else if (dir && !req.path.empty() && req.path[req.path.size() - 1] != '/')
	{
		closedir(dir);
		return (404);
	}
	else
	{
		if (errno == EACCES)
		{
			std::cerr << "403: forbidden, access denied\n";
			closedir(dir);
			return (403);
		}
		file.open(full_path.c_str());
		if (file.fail())
		{
			struct stat buffer;
			if (stat(full_path.c_str(), &buffer) == 0)
			{
				if (errno == EACCES)
				{
					std::cerr << "403: forbidden, access denied\n";
					closedir(dir);
					return (403);
				}
			}
			std::cerr << "404: invalid request path, not found\n";
			closedir(dir);
			return (404);
		}
		else
			req.path_type = "file";
		file.close();
	}
	closedir(dir);
	return (100);
}

void printBanner(){
	std::cout << std::endl;
    LOG_GREEN("░█░█░█▀▀░█▀▄░█▀▀░█░█░█▀▄░█▀▀░█▀▄░█░█░█▀▀░█▀▄░");
	LOG_GREEN("░█▄█░█▀▀░█▀▄░▀▀█░█▀█░█▀▄░█▀▀░█▀▄░▀▄▀░█▀▀░█▀▄░");
	LOG_GREEN("░▀░▀░▀▀▀░▀▀░░▀▀▀░▀░▀░▀░▀░▀▀▀░▀░▀░░▀░░▀▀▀░▀░▀░");
	std::cout << std::endl;
    std::cout  << GREEN << ITALIC << "Simple webserver made by @pgiorgi, @tjuvan, @aeid" << RESET << std::endl;
	std::cout << std::endl;
	std::cout << "Usage: ./webserver <file.conf>\n" << std::endl;
	std::cout << "+*+*+*+*+*+*+*+*+*+*++*+*+*+*+*+*+*+*+*+*+*+*+*+*+" << std::endl;
	std::cout << BOLD << "Logs color legend:" << RESET << std::endl;
	LOG_GREEN("Sockets (servers/clients)");
	LOG_YELLOW("Epoll");
	LOG_CYAN("Request");
	LOG_MAGENTA("Response");
	LOG_BLUE("Signals");
	LOG_RED("Errors");
	std::cout << "+*+*+*+*+*+*+*+*+*+*++*+*+*+*+*+*+*+*+*+*+*+*+*+*+" << std::endl;
	std::cout << std::endl;
}
