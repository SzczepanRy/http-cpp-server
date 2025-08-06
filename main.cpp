#include "parse.hpp"
#include "request.hpp"
#include "router.hpp"
#include "server.hpp"
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // Link against Winsock library

int main() {

  Server server;
  /*
    std::cout << Parse::checkStr("/assets/index.js", "/assets",
                                 std::strlen("/assets/index.js"),
                                 std::strlen("/assets/ww"))
              << "\n";
  */
  server.Init();

  // Req loop
  //
  while (true) {
    server.acceptClient();

    std::string requestBuffer;
    char buffer[1024] ={};

    while (true) {
      int bytesReceived =
          recv(server.clientSocket, buffer, sizeof(buffer) - 1, 0);
      if (bytesReceived <= 0) {
        break; // client disconnected or error
      }
      requestBuffer.append(buffer, bytesReceived);
      if (requestBuffer.find("\r\n\r\n") != std::string::npos) {
        break; // full request received
      }
    }

    int last = 0;
    std::cout << "request started" << "\n";
    Request R;

    for (int i = 0; i < requestBuffer.size(); i++) {
      if (requestBuffer[i] == '\n') {
        char temp[1024] = {};
        const char *raw = requestBuffer.c_str();
        if (raw[last] == '\n') {
          Parse::slice(raw, temp, last + 1, i - 1, 1024);
        } else {
          Parse::slice(raw, temp, last, i, 1024);
        }

        // handling the first chunk
        if (last == 0) {
          char method[64] = {};
          char path[64] = {};
          Parse::spilt(temp, sizeof(temp), method, sizeof(method), ' ', 0);
          Parse::spilt(temp, sizeof(temp), path, sizeof(path), ' ', 1);
          Parse::clearSpaces(path, 64);

          // converting to string bo gruby tak powedział
          std::string strMethod(method, Parse::len(method));
          R.Method = strMethod;
          std::string strPath(path, Parse::len(path));
          R.Path = strPath;
          /*
          Parse::copyCharArr(method, R.Method, 64);
          Parse::copyCharArr(path, R.Path, 64);
          */
        } else {
          char title[64] = {};
          char desired[6] = "Host:";
          Parse::spilt(temp, sizeof(temp), title, sizeof(title), ' ', 0);
          Parse::clearSpaces(title, 64);
          if (Parse::compareArrays(title, desired)) {
            char client[64] = {};
            Parse::getLast(temp, sizeof(temp), client, sizeof(client), ' ');
            Parse::clearSpaces(client, 64);

            // converting to string bo gruby tak powedział

            std::string strClient(client, Parse::len(client));
            R.ClientUrl = strClient;
            //              Parse::copyCharArr(client, R.ClientUrl, 64);
          }
        }
        last = i;
      }
      // here will be using router
      // initalising router and passing the instance of the request
    };


    std::cout << "req buffer ======================================\n "
              << requestBuffer << "\n===================================\n";

    R.GetBody(requestBuffer);

    /*
        std::cout << "reguest in question+++++++++\n ";
        R.print();
        */
    Router router(R, server);
    // routing the request
    router.serveHTTP();

    //  requestBuffer.clear(); // for next request
    closesocket(server.clientSocket);
    requestBuffer.clear();
  }

  // closesocket(server.clientSocket);

  closesocket(server.serverSocket);
  WSACleanup();
  return 0;
}
// g++ -o main main.cpp -lws2_32
