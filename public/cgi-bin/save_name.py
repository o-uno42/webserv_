#!/usr/bin/env python3

import cgi
import os
import urllib.parse

# Get the current directory of the script
current_dir = os.path.dirname(os.path.abspath(__file__))

# Define the file path relative to the script's directory
file_path = os.path.join(current_dir, "../cache/names.txt")

# Ensure the directory exists
os.makedirs(os.path.dirname(file_path), exist_ok=True)

# Get the form data
query_string = os.environ.get('QUERY_STRING', '')
form = urllib.parse.parse_qs(query_string)
name = form.get("name", ["Unknown"])[0]
surname = form.get("surname", ["Unknown"])[0]

# Save to a file
with open(file_path, "a") as f:
    f.write(f"{name} {surname}\n")

# Create the HTTP response body
response_body = (
    "<html><body>"
    "<h1>Success!</h1>"
    f"<p>Saved: {name} {surname}</p>"
    "</body></html>\r\n"
)

# Calculate the content length
content_length = len(response_body.encode('utf-8'))

# Print HTTP headers
print("HTTP/1.1 200 OK\r\n", end="")
print("Content-Type: text/html\r\n", end="")
print(f"Content-Length: {content_length}\r\n", end="")
print("\r\n", end="")  # End of headers

# Print the response body
print(response_body, end="")
