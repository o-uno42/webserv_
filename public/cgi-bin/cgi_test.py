#!/usr/bin/python3

import os
import urllib.parse

print("Content-Type: text/html\n")  # Important: End with two newlines

print("<html><body>")
print("<h1>CGI Script Debug</h1>")

# Get QUERY_STRING from environment
query_string = os.environ.get("QUERY_STRING", "")
print(f"<p>Query String: {query_string}</p>")
params = urllib.parse.parse_qs(query_string)  # Parses GET parameters into a dictionary

name = params.get("name", ["Unknown"])[0]
email = params.get("email", ["Unknown"])[0]

print(f"<p>Name: {name}</p>")
print(f"<p>Email: {email}</p>")

print("</body></html>")

# #!/usr/bin/env python
# import sys
# import os

# """
# RAW HTTP REQUEST THAT A CLIENT MIGHT SEND:

# POST /cgi-bin/cgi.py HTTP/1.1
# Host: example.com
# Content-Type: text/plain
# Content-Length: 18

# Hello from client!
# """

# # Get content length from environment
# try:
#     content_length = int(os.environ.get("CONTENT_LENGTH", 0))
# except ValueError:
#     content_length = 10

# # Read the request body from stdin
# post_data = sys.stdin.read(content_length) if content_length > 0 else ""

# # Output HTTP headers and response
# print("HTTP/1.1 200 OK\r\n", end="")
# print("Content-Type: text/plain\r\n", end="")
# print("Content-length: " + str(content_length) + "\r\n", end="")
# print("\r\n", end="")
# print("Hello from Python CGI script!\r\n", end="")
# print("You sent:\r\n", end="")
# print(post_data + "\r\n", end="")
