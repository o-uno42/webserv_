#!/usr/bin/python3

import cgi
import os

print("Content-Type: text/html\n")  # Required HTTP header

print("<html><body>")
print("<h1>File Upload Debug</h1>")

# # Parse form data
# form = cgi.FieldStorage()

# # Get the uploaded file
# if "file" in form:
#     file_item = form["file"]

#     if file_item.filename:
#         file_content = file_item.file.read().decode("utf-8", errors="ignore")  # Read file safely

#         print(f"<p><b>Filename:</b> {file_item.filename}</p>")
#         print(f"<p><b>File Content:</b></p>")
#         print(f"<pre>{file_content}</pre>")
#     else:
#         print("<p>Error: No file uploaded</p>")
# else:
#     print("<p>Error: 'file' field not found in form</p>")

# print("</body></html>")
