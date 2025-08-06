#include "file.hpp"
#include "llmConn.hpp"
#include "parse.hpp"
#include "request.hpp"
#include "server.hpp"
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>

class Router {
private:
  Request req;
  Server serv;

public:
  Router(const Request &r, const Server &s) : req(r), serv(s){};

  void print() {
    std::cout << "ROUTERRRR  " << req.Path << " " << req.Method << "\n";
  }

  void serveHTTP() {

    std::cout << "serving path " << req.Path << "\n";
    if (req.Method == "POST") {
      if (req.Path == "/chat") {
        std::string query = req.Body.substr(req.Body.find(":") + 1);
        std::cout << "query " << query << "\n";
        llmClient llm;
        std::string res = llm.sendQuery(query);

        /// parsing here but it shoud be cleaner in a class
        size_t headerEnd = res.find("\r\n\r\n");
        if (headerEnd != std::string::npos) {
          res = res.substr(headerEnd + 4);
        } else {
          std::cout << "could not prse body";
        }

        size_t jsonBegin = res.find_first_of("{");
        size_t jsonEnd = res.find_last_of("}");
        if (jsonBegin != std::string::npos && jsonEnd != std::string::npos &&
            jsonEnd > jsonBegin) {
          std::string resBody = res.substr(jsonBegin, jsonEnd - jsonBegin + 1);
          serv.sendHttpCharResponse(resBody.c_str(), resBody.length(),
                                    "application/json");
        } else {
          // Fallback or error handling
          serv.sendHttpCharResponse("{\"error\": \"Invalid JSON in response\"}",
                                    37, "application/json");
        }
      }

      // serv.sendHttpTestResponse();
    } else if (req.Method == "GET") {
      if (req.Path == "/") {
        std::string filePath = "./assets/index.html";
        std::vector<char> buffer = File::FileTocharArr(filePath);
        serv.sendHttpCharResponse(buffer.data(), buffer.size(), "text/html");

      } else if (req.Path == "/favicon.ico") {
        std::vector<char> buffer = File::FileTocharArr("./assets/cpp.png");
        serv.sendHttpCharResponse(buffer.data(), buffer.size(), "image/png");

      } else if (Parse::checkStr(req.Path, "/assets", sizeof(req.Path),
                                 std::strlen("/assets"))) {
        std::string tempPath = req.Path;

        size_t dotPos = tempPath.find_last_of('.');
        std::string ext =
            (dotPos != std::string::npos) ? tempPath.substr(dotPos + 1) : "";

        std::string mimeType = "text/plain";
        auto exists = req.Mime.find(ext);
        if (exists != req.Mime.end()) {
          mimeType = req.Mime.at(ext);
        }

        std::string filePath = "." + tempPath;

        std::vector<char> buffer = File::FileTocharArr(filePath);
        std::cout << "sending file " << filePath << " with mime " << mimeType
                  << '\n';

        serv.sendHttpCharResponse(buffer.data(), buffer.size(), mimeType);
      }
    } else {
      serv.sendHttpTestResponse();
    }
  }
};
