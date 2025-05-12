#!/usr/bin/env python3

import cgi
import os
import urllib.parse
import base64

response_body = (
        "<html><body>"
        "<h1>Access Denied</h1>"
        "<p>Invalid or missing authentication token.</p>"
        "</body></html>"
        )

correct_username = "admin"
correct_password = "password123"

cookie_header = os.environ.get('HTTP_COOKIE', '')

auth_string = urllib.parse.parse_qs(cookie_header)
if auth_string:
    auth_token = auth_string.get("auth_token", ["unknown"])[0]
    decoded_token= base64.b64decode(auth_token).decode('utf-8')
    if decoded_token != "unknown":
        parts = decoded_token.split(':')
        if len(parts) == 2:
            username, password = parts
            if  username == correct_username and password == correct_password:
                response_body = (
                        "<html><body>"
                        "<h1>Admin Panel</h1>"
                        "<p>Welcome, admin!</p>"
                        "</body></html>"
                        )

    else:
        response_body = (
                "<html><body>"
                "<h1>Access Denied</h1>"
                "<p>Incorrect username or password.</p>"
                f"<p>auth: {auth_token}"
                "</body></html>"
                )

Content_Length = len(response_body.encode('utf-8'))

print("HTTP/1.1 200 OK\r\n", end="")
print("Content-Type: text-html\r\n", end="")
print(f"Content-Length: {Content_Length}\r\n\r\n", end="")
print(response_body, end="")

