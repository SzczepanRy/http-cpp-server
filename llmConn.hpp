#include <cstddef>
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // Link against Winsock library
class llmClient {
public:
  WSADATA wsaData;
  sockaddr_in llmAddr{};
  int llmSize = sizeof(llmAddr);
  SOCKET llmSocket;

  std::string host = "127.0.0.1";
  int port = 11434;

  llmClient() {
    std::cout << " llm init started\n";
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
      std::cerr << "WSAStartup failed\n";
      abort();
    }

    this->llmSocket = this->createTcpSocket();

    llmAddr.sin_family = AF_INET;
    llmAddr.sin_port = htons(11434);
    inet_pton(AF_INET, host.c_str(), &llmAddr.sin_addr);
    connectToLLM();
  }

  std::string sendQuery(std::string query) {
    std::string body =
        R"({"model": "tinyllama", "prompt":)" + query + R"(, "stream": false})";
    std::string request = "POST /api/generate HTTP/1.1\r\n"
                          "Host: localhost:11434\r\n"
                          "Content-Type: application/json\r\n"
                          "Content-Length: " +
                          std::to_string(body.size()) +
                          "\r\n"
                          "Connection: close\r\n\r\n" +
                          body;

    std::cout << "request " << request << "\n";
    send(llmSocket, request.c_str(), request.length(), 0);

    std::string responseBuffer;
    char buffer[1024] = {};
    int contentLength = -1;
    bool Parsed = false;
    while (true) {
      int bytesReceived = recv(llmSocket, buffer, sizeof(buffer) - 1, 0);
      if (bytesReceived > 0) {

        responseBuffer.append(buffer, bytesReceived);
        size_t headerEnd = responseBuffer.find("\r\n\r\n");
        if (headerEnd != std::string::npos) {
          size_t bodyStart = responseBuffer.find("\r\n\r\n") + 4;
          size_t bodyLength = responseBuffer.size() - bodyStart;
          if (bodyLength >= contentLength) {
            std::cout << "got full response \n";
            break;
          }
        }
      } else if (bytesReceived == 0) {
        std::cout << " connection closed gracefully by the peer \n";
        break;
      } else {
        // recv error
        perror("recv failed");
        break;
      }
    }
    // this is questtionable  frfr no cap
    closesocket(llmSocket);
    WSACleanup();

    return responseBuffer;
    //    Server::sendHttpCharResponse(responseBuffer.c_str() ,
    //    responseBuffer.length() , "plainText" )
  }

  void connectToLLM() {
    if (connect(llmSocket, (SOCKADDR *)&llmAddr, llmSize) == SOCKET_ERROR) {
      std::cerr << "Connection failed\n";
      closesocket(llmSocket);
      WSACleanup();
      abort();
    }
  }

  SOCKET createTcpSocket() {
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
      std::cerr << "Socket creation failed: " << WSAGetLastError() << "\n";
      WSACleanup();
      abort();
    }
    return serverSocket;
    std::cout << "created tcp serverSocket\n";
  }
};
