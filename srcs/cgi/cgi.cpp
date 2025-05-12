/** * ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thiew <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 12:08:22 by thiew             #+#    #+#             */
/*   Updated: 2025/05/12 19:17:35 by thiew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cgi.hpp"
# include "../../includes/colors.hpp"

const 		rlim_t CGI_CPU_LIMIT = 5;
void		freeMtx(char **mtx);
void 		freeAllCgi(t_cgi &cgi);
char		**mapToCMtx(std::map<std::string, std::string> map_env);
std::string	cgi_path(Server &server);

t_cgi	envCreation(Server &server, t_request &req);

/** 
	* @description:	function that executes the cgi scripts in a fork
   					request is sent to fork as env in execve(... , ... , env);
					body is sent as stdin to fork
					create a temporary files for I/O and store  the stdin in infile
					and stdout to outfile
	* @params: server class and request struct -- everything is stored in cgi struct
	* @calls: envCreation which creates the env map
	* @called by: which ever funct handles the sockets i guess
	* @returns: a response in a form of std::string with the status code present already
*/
// std::string cgiHandler(Server &server, t_request &req)
// {
// 	pid_t	pid;
// 	t_cgi	cgi = envCreation(server, req);
// 	cgi.save_in = dup(STDIN_FILENO);
// 	cgi.save_out = dup(STDOUT_FILENO);
// 	cgi.file_in = tmpfile();
// 	cgi.file_out = tmpfile();
// 	cgi.fd_in = fileno(cgi.file_in);
// 	cgi.fd_out = fileno(cgi.file_out);

// 	write(cgi.fd_in, req.body.c_str(), req.body.size());
// 	lseek(cgi.fd_in, 0, SEEK_SET);

// 	if ((pid = fork()) < 0)
// 	{
// 		Error::runningError("Fork crashed\n");
// 		freeAllCgi(cgi);
// 		return ("Status: 500\r\n\r\n");
// 	}
	
// 	else if (!pid)
// 	{
// 		dup2(cgi.fd_in, STDIN_FILENO);
// 		dup2(cgi.fd_out, STDOUT_FILENO);
// 		char *const args[] = { NULL };
// 		execve(cgi.env.find("SCRIPT_NAME")->second.c_str(), args, cgi.env_c_mtx);

// 		Error::runningError("Child in Fork crashed\n");
// 		freeAllCgi(cgi);
// 		return ("Status: 500\r\n\r\n");
// 	}

// 	else
// 	{
// 		wait(NULL);
// 		fsync(cgi.fd_out);
// 		lseek(cgi.fd_out, 0 ,SEEK_SET);

// 		char buff[4096];
// 		std::string result;
// 		if (fgets(buff, sizeof(buff), cgi.file_out) != NULL)
// 			result = std::string(buff);
// 		else
// 		{
// 			Error::runningError("reading form child failed\n");
// 			freeAllCgi(cgi);
// 			return ("Status: 500\r\n\r\n");
// 		}
// 		freeAllCgi(cgi);
// 		return (result);

// 	}
// }


std::string cgiHandler(Server &server, t_request &req)
{
    pid_t pid;
    t_cgi cgi = envCreation(server, req);
    cgi.save_in = dup(STDIN_FILENO);
    cgi.save_out = dup(STDOUT_FILENO);
    cgi.file_in = tmpfile();
    cgi.file_out = tmpfile();
    cgi.fd_in = fileno(cgi.file_in);
    cgi.fd_out = fileno(cgi.file_out);

    // Write request body to input file
    write(cgi.fd_in, req.body.c_str(), req.body.size());
    lseek(cgi.fd_in, 0, SEEK_SET);

    if ((pid = fork()) < 0)
    {
        Error::runningError("Fork crashed\n");
        freeAllCgi(cgi);
       std::ostringstream oss;
        oss << "HTTP/1.1 500 Internal Server Error\r\n"
            << "Content-Type: text/plain\r\n"
            << "Content-Length: " << req.body.size() << "\r\n\r\n";
        return oss.str();
    }
    else if (!pid)
    {
        // Child process
        
        // Close all inherited file descriptors except those we need
        int max_fd = sysconf(_SC_OPEN_MAX);
        for (int i = 3; i < max_fd; i++) {
            if (i != cgi.fd_in && i != cgi.fd_out && 
                i != cgi.save_in && i != cgi.save_out) {
                close(i);
            }
        }
        
        // Redirect stdin/stdout
        dup2(cgi.fd_in, STDIN_FILENO);
        dup2(cgi.fd_out, STDOUT_FILENO);
        
        // Close the original descriptors after duplication
        close(cgi.fd_in);
        close(cgi.fd_out);
		std::string script_path = req.full_path; //+ cgi.env.find("SCRIPT_NAME")->second;

		//limit
		struct rlimit rl;
		rl.rlim_cur = CGI_CPU_LIMIT;  // soft limit
		rl.rlim_max = CGI_CPU_LIMIT;  // hard limit
		if (setrlimit(RLIMIT_CPU, &rl) < 0)
		{
			const char* error_msg = "Status: 500\r\nContent-Type: text/plain\r\n\r\nCGI Execution Failed\n";
			write(STDOUT_FILENO, error_msg, strlen(error_msg));
			freeAllCgi(cgi);
			exit(1);
		}

        
        char *const args[2] = { (char *)req.full_path.c_str(), NULL }; //
        execve(req.full_path.c_str() , args, cgi.env_c_mtx);

        // If execve fails, write an error message to stdout (which is redirected)
        const char* error_msg = "Status: 500\r\nContent-Type: text/plain\r\n\r\nCGI Execution Failed\n";
        write(STDOUT_FILENO, error_msg, strlen(error_msg));
		freeAllCgi(cgi);
        // Exit with error code
        exit(1);
    }
    else
    {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
		pid_t w = waitpid(pid, &status, WNOHANG);
        if (w == pid)
		{
			Error::runningError("Hanging cgi script");
			freeAllCgi(cgi);
			std::ostringstream oss;
			oss << "HTTP/1.1 500 Internal Server Error\r\n"
				<< "Content-Type: text/plain\r\n"
				<< "Content-Length: " << req.body.size() << "\r\n\r\n";
			return oss.str();
        }
        
        // Check if child exited normally
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            std::cerr << "CGI script exited with error: " << WEXITSTATUS(status) << std::endl;
        }
        
        // Seek to beginning of output file
        lseek(cgi.fd_out, 0, SEEK_SET);

        // Read the entire CGI output
        std::string result;
        char buffer[4096] = "";
        ssize_t bytes_read;
        
        while ((bytes_read = read(cgi.fd_out, buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0';
            result.append(buffer);
        }
        // std::cout << YELLOW << "CGI BUF!!!!! "  << buffer << RESET << std::endl;
        
        // If no output was generated, return an error
        if (result.empty()) {
            Error::runningError("No output from CGI script\n");
            freeAllCgi(cgi);
            std::ostringstream oss;
            oss << "HTTP/1.1 500 Internal Server Error\r\n"
                << "Content-Type: text/plain\r\n"
                << "Content-Length: " << req.body.size() << "\r\n\r\n";
            return oss.str();
        }
        
        freeAllCgi(cgi);
        // std::cout << BLUE << "CGI RES!!! : "<< result << " ::::END RES" << RESET << std::endl;
        return result;
    }
}

/** helper functions 
	* called by: cgiHandler
*/
void	freeMtx(char **mtx)
{
	size_t i = 0;
	while (mtx[i] != NULL)
		 free(mtx[i++]);
	free(mtx);
}

/* helper functions
	* called by: cgiHandler
*/
void freeAllCgi(t_cgi &cgi)
{
	dup2(cgi.save_in, STDIN_FILENO);
	dup2(cgi.save_out, STDOUT_FILENO);
	freeMtx(cgi.env_c_mtx);
	fclose(cgi.file_in);
	fclose(cgi.file_out);
	close(cgi.fd_in);
	close(cgi.fd_out);
	close(cgi.save_out);
	close(cgi.save_in);
}

/** 
   * @description: gets the headers from t_request struct and fills them into
   * an env hashmap, which will be passed to execve(cgi script) as env
   * @params: server class and t_request struct
   * @called by: 
   * @calls: mapToCMtx
   * @returns: a t_cgi struct that already has the env and env_c_mtx filled
*/
t_cgi	envCreation(Server &server, t_request &req)
{
	t_cgi	cgi;
	
	// TODO need to do the function that will check the correct path!
	//filling env hashmap
	cgi.env["REDIRECT_STATUS"] = "200"; 
	cgi.env["GATEWAY_INTERFACE"] = "CGI/1.1";
	cgi.env["SCRIPT_NAME"] = req.parsed_path;
	cgi.env["SCRIPT_FILENAME"] = req.full_path;
	cgi.env["REQUEST_METHOD"] = req.method;
	cgi.env["CONTENT_LENGTH"] = req.content_length ? std::to_string(req.content_length) : std::to_string(server.getClientMax());
	cgi.env["CONTENT_TYPE"] = req.content_type;
	cgi.env["PATH_INFO"] = "";
	cgi.env["PATH_TRANSLATED"] = req.full_path;
	cgi.env["REMOTEaddr"] = server.getHost(); // TODO
	cgi.env["QUERY_STRING"] = req.path_query;
	cgi.env["REMOTE_IDENT"] = "";
	cgi.env["REMOTE_USER"] = "";
	cgi.env["REQUEST_URI"] = req.path;
	cgi.env["HTTP_COOKIE"] = req.cookies;
	if (req.host != "")
		cgi.env["SERVER_NAME"] = server.getServerName();
	else
		cgi.env["SERVER_NAME"] = server.getHost();
	// cgi.env["SERVER_PORT"] = server.getPort();
	cgi.env["SERVER_PROTOCOL"] = "HTTP/1.1";
	cgi.env["SERVER_SOFTWARE"] = "WeebServ/1.0";

	//transform map to char matrix -- dynamically allocated
	cgi.env_c_mtx = mapToCMtx(cgi.env);

	// std::cout << RED << "THIS IS CGI PRINT\n";
	// for (std::map<std::string, std::string>::iterator it = cgi.env.begin();
	// 		it != cgi.env.end(); it++)
	// 	std::cout << it->first << ", " << it->second << "\n";
	// std::cout << RESET;

	return (cgi);
}

// std::string	cgi_path(Server &server)
// {
// 	std::string res;


// 	return (res);
// }

/** 
	* @description : transforms a string map into a c style char **matrix
	* @params : already filled map of request header params
	* @called by : envCreation
	* @returns: a c style char **matrix
*/
char	**mapToCMtx(std::map<std::string, std::string> map_env)
{
	char		**char_env = (char **)malloc(sizeof(char *) * (map_env.size() + 1));
	std::string	tmp;
	int			i = 0;

	for (std::map<std::string, std::string>::iterator it = map_env.begin(); 
			it != map_env.end(); it ++)
	{
		tmp = it->first + "=" + it->second;
		char_env[i++] = strdup(tmp.c_str());
		tmp.clear();
	}

	char_env[i] = NULL;
	return (char_env);
}
