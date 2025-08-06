#include <cstring>
#include <iostream>
#include <map>
#include <string>
#pragma once

class Request {
public:
  std::string Path;
  std::string Method;
  std::string ClientUrl;
  std::string Body;

  std::map<std::string, std::string> Mime = {
      {"html", "text/html"},
      {"css", "text/css"},
      {"js", "application/javascript"},
      {"png", "image/png"},
  };

  void print() {
    std::cout << "print : " << Path << Method << ClientUrl << "\n";
  }

  void GetBody(std::string req) {
    size_t headerEnd = req.find("\r\n\r\n");
    if (headerEnd != std::string::npos) {

      Body = req.substr(headerEnd + 4);
      if (Body.size() > 0) {
        Body = Body.substr(1, Body.size() - 2);
      }
    } else {
      std::cout << "could not prse body";
    }
  }
};
