#!/usr/bin/env node

/*
RAW HTTP REQUEST THAT A CLIENT MIGHT SEND:

POST /cgi-bin/cgi.js HTTP/1.1
Host: example.com
Content-Type: text/plain
Content-Length: 25

This is from the client!
*/

const fs = require('fs');
const querystring = require('querystring');
try {
  figlet = require('figlet');
} catch (err) {
  const errorMsg = `Error: Cannot find module 'figlet'.\n${err.message}`;
  process.stdout.write("HTTP/1.1 500 Internal Server Error\r\n");
  process.stdout.write("Content-Type: text/plain; charset=UTF-8\r\n\r\n");
  process.stdout.write(errorMsg + "\n");
  process.exit(1);
}


process.stdin.setEncoding('utf8');

let body = "";
process.stdin.on("data", (chunk) => {
  body += chunk;
});

function figletPromise(text) {
  return new Promise((resolve, reject) => {
    figlet(text, (err, data) => {
      if (err) {
        reject(err);
      } else {
        resolve(data);
      }
    });
  });
}

process.stdin.on("end", async () => {
  const filePath = process.cwd() + "/public/cache/jsFile.txt";
  let data = querystring.parse(body).data;
  body = data ? data : body;

  let figletText;
  try {
    figletText = await figletPromise(body);
    // Handle too-long input manually if needed
    if (body.length > 20) {
      throw new Error(`Input too long: ${body.length} characters (max 20)`);
    }
    // Convert newlines for HTML
    figletText = figletText.replace(/\n/g, "<br>");
  } catch (err) {
    // Write error response and exit
    const errorMsg = `Error generating ASCII art: ${err.message}`;
    process.stdout.write("HTTP/1.1 500 Internal Server Error\r\n");
    process.stdout.write("Content-Type: text/plain; charset=UTF-8\r\n\r\n");
    process.stdout.write(errorMsg + "\n");
    process.exit(1);
  }

  const body_response = `<html>
<head>
  <title>cgi.js response</title>
</head>
<body>
  <h1> response message: </h1>
  <p>
    file saved to ${filePath}
  </p>
  <pre>${figletText}</pre>
</body>
</html>`;
  const content_len = Buffer.byteLength(body_response, 'utf8');
  const headers = `Content-Type: text/html; charset=UTF-8\r\nContent-length: ${content_len}\r\nServer: Webserv\r\n`;

  fs.writeFile(filePath, figletText, 'utf8', (err) => {
    if (err) {
      process.stdout.write("HTTP/1.1 500 Internal Server Error\r\n");
      process.stdout.write("Content-Type: text/plain; charset=UTF-8\r\n\r\n");
      process.stdout.write(`Error saving file: ${err.message}\n`);
      process.exit(1);
    } else {
      process.stdout.write("HTTP/1.1 200 OK\r\n");
      process.stdout.write(headers + "\r\n");
      process.stdout.write(body_response + "\r\n");
    }
  });
});

