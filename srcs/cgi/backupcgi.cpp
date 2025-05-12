
// std::string cgiHandler(Server &server, t_request &req)
// {
//     pid_t pid;
//     t_cgi cgi = envCreation(server, req);
//     cgi.save_in = dup(STDIN_FILENO);
//     cgi.save_out = dup(STDOUT_FILENO);
//     cgi.file_in = tmpfile();
//     cgi.file_out = tmpfile();
//     cgi.fd_in = fileno(cgi.file_in);
//     cgi.fd_out = fileno(cgi.file_out);

//     // Write request body to input file
//     write(cgi.fd_in, req.body.c_str(), req.body.size());
//     lseek(cgi.fd_in, 0, SEEK_SET);

//     if ((pid = fork()) < 0)
//     {
//         Error::runningError("Fork crashed\n");
//         freeAllCgi(cgi);
//         return ("Status: 500\r\n\r\n");
//     }
//     else if (!pid)
//     {
//         // Child process
        
//         // Close all inherited file descriptors except those we need
//         int max_fd = sysconf(_SC_OPEN_MAX);
//         for (int i = 3; i < max_fd; i++) {
//             if (i != cgi.fd_in && i != cgi.fd_out && 
//                 i != cgi.save_in && i != cgi.save_out) {
//                 close(i);
//             }
//         }
        
//         // Redirect stdin/stdout
//         dup2(cgi.fd_in, STDIN_FILENO);
//         dup2(cgi.fd_out, STDOUT_FILENO);
        
//         // Close the original descriptors after duplication
//         close(cgi.fd_in);
//         close(cgi.fd_out);
//         // /home/uno/WEBSERV/public/cgi-bin/get.py
// 		//DEBUGGING
// 		std::string script_path = "/home/uno/WEBSERV/public" + cgi.env.find("SCRIPT_NAME")->second;
//         // std::cout << " CGI PATH SCRIPT : " << script_path << std::endl;
// 		// std::cout << "Attempting to execute CGI script: " << script_path << std::endl;

// 		// Check if the file exists and is executable
// 		// if (access(script_path.c_str(), F_OK) != 0) {
// 		// 	std::cerr << "Error: CGI script does not exist: " << script_path << std::endl;
// 		// } else if (access(script_path.c_str(), X_OK) != 0) {
// 		// 	std::cerr << "Error: CGI script is not executable: " << script_path << std::endl;
// 		// }
// 		//DEBUGGING

        
//         // char *const args[] = { NULL }; //
//         char *const args[] = { (char*)script_path.c_str(), NULL };
//         std::string script_name = "/home/uno/WEBSERV/public" + cgi.env.find("SCRIPT_NAME")->second;
//         // std::cout << " SCRIPT NAME : " + script_name << std::endl;
//         execve( "/home/uno/WEBSERV/public/cgi-bin/get.py" , args, cgi.env_c_mtx);

//         // If execve fails, write an error message to stdout (which is redirected)
//         const char* error_msg = "Status: 500\r\nContent-Type: text/plain\r\n\r\nCGI Execution Failed\n";
//         write(STDOUT_FILENO, error_msg, strlen(error_msg));
        
//         // Exit with error code
//         exit(1);
//     }
//     else
//     {
//         // Parent process
//         int status;
//         waitpid(pid, &status, 0);
        
//         // Check if child exited normally
//         if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
//             std::cerr << "CGI script exited with error: " << WEXITSTATUS(status) << std::endl;
//         }
        
//         // Seek to beginning of output file
//         lseek(cgi.fd_out, 0, SEEK_SET);

//         // Read the entire CGI output
//         std::string result;
//         char buffer[4096];
//         ssize_t bytes_read;
        
//         while ((bytes_read = read(cgi.fd_out, buffer, sizeof(buffer) - 1)) > 0) {
//             buffer[bytes_read] = '\0';
//             result.append(buffer);
//         }
//         std::cout << YELLOW << "CGI BUF!!!!! "  << buffer << RESET << std::endl;
        
//         // If no output was generated, return an error
//         if (result.empty()) {
//             Error::runningError("No output from CGI script\n");
//             freeAllCgi(cgi);
//             return ("Status: 500\r\n\r\n");
//         }
        
//         freeAllCgi(cgi);
//         std::cout << BLUE << "CGI RES!!! : "<< result << RESET << std::endl;
//         return result;
//     }
// }