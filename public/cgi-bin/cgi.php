#!/usr/bin/env php
<?php
$method = $_SERVER['REQUEST_METHOD'];

if ($method === 'GET') {
    // For GET requests, data is in $_GET
    $data = $_GET;
    $len = 0;
} else { // Assume POST (or others with a body)
    $len = isset($_SERVER['CONTENT_LENGTH']) ? $_SERVER['CONTENT_LENGTH'] : 0;
    $input = file_get_contents("php://stdin");
    parse_str($input, $data);
}

//response headers
header("Content-Type: text/html; charset=UTF-8");
$header = "HTTP/1.1 200 OK\r\n";
$header .= "Content-Type: text/html; charset=UTF-8\r\n";

//valid credentials
$valid_username = "admin";
$valid_password = "password123";

$auth_token = isset($_COOKIE['auth_token']) ? $_COOKIE['auth_token'] : null;

// debug output
$debug_output = "<p>Received username: " . (isset($data["username"]) ? htmlspecialchars($data["username"]) : "") . "</p>\n";
$debug_output .= "<p>Valid username: " . $valid_username . "</p>\n";
$debug_output .= "<p>Received password: " . (isset($data["password"]) ? htmlspecialchars($data["password"]) : "") . "</p>\n";
$debug_output .= "<p>Valid password: " . $valid_password . "</p>\n";
$debug_output .= "<p>Content Length: " . $len . "</p>\n";
$debug_output .= "<p>Stored Cookie: " . htmlspecialchars($auth_token) . "</p>\n";


//main output
$out = "<html>\n<head>\n<title>PHP CGI Auth Check</title>\n</head>\n<body>\n";
$out .= "<h1>This is an auth check in PHP!</h1>\n";
$out .= $debug_output;

if (isset($data["username"]) && isset($data["password"])) {
    if ($data["username"] === $valid_username && $data["password"] === $valid_password) {

		$auth_token = base64_encode($data["username"] . ":" . $data["password"]);
		setcookie("auth_token", $auth_token, time() + 3600, "/");
		$header .= "Set-Cookie: auth_token=$auth_token; Path=/; Max-Age=3600\r\n";

        $out .= "<h1 style='color: green;'>Authentication successful!<br>Hello " . htmlspecialchars($data["username"]) . "</h1>\n";
    } else {
        $out .= "<h1 style='color: red;'>Invalid username or password.</p>\n";
    }
} else {
    $out .= "<h1 style='color: orange;'>Missing username or password.</p>\n";
}

$out .= "</body>\n</html>";
$header .= "Content-Length: " . strlen($out) . "\r\n";

// Now output complete HTTP response headers and body
/* echo "HTTP/1.1 200 OK\r\n"; */
/* echo "Content-Type: text/html; charset=UTF-8\r\n"; */
/* echo "Content-Length: " . strlen($out) . "\r\n"; */
/* echo "\r\n"; */

echo $header;
echo "\r\n";
echo $out;
echo "\r\n";
?>
