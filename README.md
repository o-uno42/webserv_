link for meetings:
https://vidra.radiostudent.si/webserv

# TODO:
-multiple servers in config -petre
        Setup multiple servers with different ports. X
        Setup multiple servers with different hostnames (use something like: curl --resolve example.com:80:127.0.0.1 http://example.com/).
- try running in browser differn to ports with different indexes. -petre X
-signal iterception -petre X
-autoindex - list directories - tjaz
-allowed methods - tjaz
-default error pages - petre
-default file in route path ex: when get /public/ it should return /public/index.html - tjaz

### when we finished working on the above TODOs
-redirected urls 
-write tests for correction

# errors to resolve
amir:
- getAutoIndex() in server.cpp is seg faulting - i think when no autoindex in location
- other configs are seg faulting
- getErrorPage() not working correctly - now returns full apth or undefined if wrong in config

- Socket::launchingServer(Server) (socketHandler.cpp:78); Invalid write of size 4 if bind or listen fails (try port 8080) / Poi resta aperto (socketHandler 58) X

-redirect URL

-better cleanup? chiudi socket se è ANCORA aperto (inutile)

-Test sui server names X

# how the location paths should behave
```
example config:
 server 

 {
     host 127.0.0.1;
     port 3001;
     server_name myserver.com;
     error_page 404 /error_page/error.html;
     client_max_body_size 10m;
     root /public;

     location /error_page/
     {
         index index.html;
         allowed_methods GET POST;
     }
         location /database/ {
         root /test
         index index.html;
         allowed_methods GET POST;
     }
 }


get getLocationPath() works correctly and it returns /error_page/ or /database/ which is correct

getRootLocation() is not working as it should be.

this is what it should return in case /error_page/ : /home/thiew/webserv/public
in case /database/ : /home/thiew/webserv/test
now its also some problem and it returns this:гЕ[erv/public/database/
so 1. there is a problem getting the full local path. 
2. it should not append the location itself. just its root dir.
```





# uniform syntax rules/conventions
```
- use the same syntax rules for all the code
- use the same naming conventions for all the code
- use the same indentation for all the code
- use the same comments for all the code
- use the same error handling for all the code
- use the same logging for all the code
- use the same testing for all the code
```
### naming conventions
```
- use the cpp modules naming conventions for classes and as much C conventions for functions
- use the same naming conventions for all the code
- variables: snake_case 
- functions: camelCase
- classes: CamelCase
- private variables: _snake_case
```
### indentation
```
- width of indentation: 4 spaces
- width of tab: 4 spaces
```
### comments
```
- inside the code: // comment
- outside the code: /* comment */
- each function or group of functions that work together should have a comment explaining:
    - what the function does
    - what the function returns
    - what the function parameters are
    - what the function calls (which functions it calls)
    - what the function is called by (which functions call it)
    - if its a group of functions explain how they work together
```
```example
/*
@description: adds two numbers
@returns: the sum of the two numbers
@params: two ints a, b
@calls: none
@called by: main
*/
```

### error handling
```
- uniform error handling for all the code, so you understand the error handling of the code you have not written
```


# structure of the program
```
error handling
config file
reading the config file
logging
server
test client
rest requests
cgi
tests


```
structure of the program:
```
each more complex part should be developed in a separate branch and directory.
# Webserver Project Directory Structure (example)
webserver/
├── Makefile        # Makefile for building the project
├── README.md       # Project README
├── executable      # Executable file
├── includes/       # Header files (.h/.hpp)
│   ├── main.hpp
│   ├── server.hpp
│   ├── utils.hpp
│   └── handler.hpp
├── srcs/           # Source files (.cpp)
│   ├── main.cpp
│   ├── server/
│   │   └── server.cpp
│   ├── utils/
│   │   └── utils.cpp
│   └── handlers/
│       └── handler.cpp
└── obj/            # Object files (.o)
   ├── server/
   │   └── server.o
   ├── utils/
   │   └── utils.o
   └── handlers/
       └── handler.o

makefile options: make all, make noextra, make clean, make clean_noextra make fclean, make re
# added noextra to compile without extra flags (--Wall, --Wextra, --Werror)
in theory makefile should be able to compile files also in subdirectories(did not test this)
```

### error handling
```
library/class with error handling functions or class with exception handling, such as:
- throw error
- catch error
- log error
- some that quit the program
- some that continue the program
- example:
    - a function that checks for errors(sys calls), prints error,exception,whatever
    - wrapper function for this one that decideds based on the exception or syscall to terminate or continue

Need to catch errno or system defined exceptions, and add our own error codes and exceptions.

```

### config file
```
config file with the following parameters:
- server port
- server ip
- server root directory
- server log file
- ....
```

### reading the config file
```
read the config file and store the parameters in a class or map?
```

### logging
```
log the server activities in a log file
```

### server
```
server class with the following functions:
- start
- stop
- restart
- handle requests
- ....
```

### test client
```
test client class with the following functions:
- send requests to the server
- receive responses from the server
- ....
```

### rest requests
```
class with the following functions:
- get
- post
- put
- delete

```

### cgi
```
class with the following functions:
- execute cgi scripts
- ....
```

### tests
```
test the server with the test client.
- functions to automatically test the server
```
