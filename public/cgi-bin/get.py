#!/usr/bin/env python3
import cgi

# Get query parameters
# form = cgi.FieldStorage()
# name = form.getvalue("name", "Unknown")
# email = form.getvalue("email", "No email")

# Print HTTP response
print("Content-Type: text/html\n")
print(f"<html><body>")
print(f"<h1>Form Submitted</h1>")
# print(f"<p>Name: {name}</p>")
# print(f"<p>Email: {email}</p>")
print(f"</body></html>")