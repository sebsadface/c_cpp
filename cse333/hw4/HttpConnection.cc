/*
 * Copyright ©2023 Justin Hsia.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2023 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */
#include <stdint.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <map>
#include <string>
#include <vector>

#include "./HttpRequest.h"
#include "./HttpUtils.h"
#include "./HttpConnection.h"

using std::map;
using std::string;
using std::vector;

namespace hw4 {
static const char* kHeaderEnd = "\r\n\r\n";
static const int kHeaderEndLen = 4;
static const int buf_len = 1024;

bool HttpConnection::GetNextRequest(HttpRequest* const request) {
  // Use WrappedRead from HttpUtils.cc to read bytes from the files into
  // private buffer_ variable. Keep reading until:
  // 1. The connection drops
  // 2. You see a "\r\n\r\n" indicating the end of the request header.
  //
  // Hint: Try and read in a large amount of bytes each time you call
  // WrappedRead.
  //
  // After reading complete request header, use ParseRequest() to parse into
  // an HttpRequest and save to the output parameter request.
  //
  // Important note: Clients may send back-to-back requests on the same socket.
  // This means WrappedRead may also end up reading more than one request.
  // Make sure to save anything you read after "\r\n\r\n" in buffer_ for the
  // next time the caller invokes GetNextRequest()!

  // STEP 1:
  int res;
  unsigned char buf[buf_len];

  // Keep reading to buffer_ until we found "\r\n\r\n"
  while (buffer_.find(kHeaderEnd) == string::npos) {
    res = WrappedRead(fd_, buf, buf_len);
    if (res == -1) {
      // Read failed, return false
      return false;
    }
    if (res == 0) {
      // We reached EOF, stop reading
      break;
    }

    // Append reading results to buffer_
    buffer_ += string(reinterpret_cast<char*>(buf), res);
  }

  // Check if we read to the end of the request
  // header. (Checking if we can find "\r\n\r\n" in buffer_)
  size_t end = buffer_.find(kHeaderEnd);
  if (end == string::npos) {
    // Invalid read, return false.
    return false;
  }

  // We sucessfully read the reaquest, now parse the request header
  *request = ParseRequest(buffer_.substr(0, end + kHeaderEndLen));

  // Preserve the request body (contents after "\r\n\r\n")
  buffer_ = buffer_.substr(end + kHeaderEndLen);

  if (request->uri() == "") {
    // Request header is malformed, return false
    return false;
  }

  return true;
}

bool HttpConnection::WriteResponse(const HttpResponse& response) const {
  string str = response.GenerateResponseString();
  int res = WrappedWrite(
      fd_, reinterpret_cast<const unsigned char*>(str.c_str()), str.length());
  if (res != static_cast<int>(str.length())) return false;
  return true;
}

HttpRequest HttpConnection::ParseRequest(const string& request) const {
  HttpRequest req("/");  // by default, get "/".

  // Plan for STEP 2:
  // 1. Split the request into different lines (split on "\r\n").
  // 2. Extract the URI from the first line and store it in req.URI.
  // 3. For the rest of the lines in the request, track the header name and
  //    value and store them in req.headers_ (e.g. HttpRequest::AddHeader).
  //
  // Hint: Take a look at HttpRequest.h for details about the HTTP header
  // format that you need to parse.
  //
  // You'll probably want to look up boost functions for:
  // - Splitting a string into lines on a "\r\n" delimiter
  // - Trimming whitespace from the end of a string
  // - Converting a string to lowercase.
  //
  // Note: If a header is malformed, skip that line.

  // STEP 2:

  // vector for all the lines in request header
  vector<string> lines;
  // vector for all the elements in the first line of request
  vector<string> first_line;

  // Split the request into different lines(split on "\r\n")
  boost::split(lines, request, boost::is_any_of("\r\n"),
               boost::token_compress_on);

  // Split the first line into different elements (split on " ")
  boost::split(first_line, lines.front(), boost::is_any_of(" "),
               boost::token_compress_on);

  // Check if the first line is valid
  if (first_line.size() != 3 || first_line.front() != "GET" ||
      first_line.back() != "HTTP/1.1") {
    // The first line is malformed, return an empty uri.
    req.set_uri("");
    return req;
  }

  req.set_uri(first_line[1]);

  // For the rest of the lines in the request, track the header name and value
  // and store them in req.headers_
  for (size_t i = 1; i < lines.size(); i++) {
    size_t delim = lines[i].find(": ");
    if (delim != string::npos) {
      string header_name = lines[i].substr(0, delim);
      boost::to_lower(header_name);

      string header_val = lines[i].substr(delim + sizeof(": ") - 1);

      req.AddHeader(header_name, header_val);
    }
  }
  return req;
}
}  // namespace hw4
