#include <cstring>
#include <iostream>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // Link against Winsock library
#pragma once
class Server {
public:
  WSADATA wsaData;
  sockaddr_in serverAddr{};
  sockaddr_in clientAddr{};
  int clientSize = sizeof(clientAddr);
  SOCKET serverSocket;
  SOCKET clientSocket;

  void Init() {
    std::cout << "init started\n";
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
      std::cerr << "WSAStartup failed\n";
      abort();
    }

    this->serverSocket = this->createTcpSocket();

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
    serverAddr.sin_port = htons(8080);       // Port 8080
                                             //
    BindSocket();
    ListenSocket();
  //  acceptClient();
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

  void BindSocket() {
    if (bind(serverSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr)) ==
        SOCKET_ERROR) {
      std::cerr << "Bind failed: " << WSAGetLastError() << "\n";
      closesocket(serverSocket);
      WSACleanup();
      abort();
    }
  }
  void ListenSocket() {
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
      std::cerr << "Listen failed: " << WSAGetLastError() << "\n";
      closesocket(serverSocket);
      WSACleanup();
      abort();
    }
    std::cout << "Server listening on port 8080...\n";
  }

  void acceptClient() {

    this->clientSocket =
        accept(serverSocket, (SOCKADDR *)&clientAddr, &clientSize);
    if (this->clientSocket == INVALID_SOCKET) {
      std::cerr << "Accept failed: " << WSAGetLastError() << "\n";
      closesocket(serverSocket);
      WSACleanup();
      abort();
    }
    std::cout << "Client connected.\n";
  }

  void sendHttpTestResponse() {
    const char *httpResponse = "HTTP/1.1 200 OK\r\n"
                               "Content-Type: text/plain\r\n"
                               "Content-Length: 13\r\n"
                               "\r\n"
                               "Hello, world!";

    int result =
        send(this->clientSocket, httpResponse, strlen(httpResponse), 0);
    std::cout << "write\n";
    if (result == SOCKET_ERROR) {
      std::cerr << "Send failed: " << WSAGetLastError() << "\n";
      closesocket(clientSocket);
    } else {
      std::cout << "Sent " << result << " bytes.\n";
    }
  }


  void sendHttpCharResponse(const char* data, size_t dataSize , std::string type) {

    std::string httpResponseHeader = "HTTP/1.1 200 OK\r\n"
                                "Content-Type: "+type+"\r\n"
                                "Content-Length: " +
                                std::to_string(dataSize) +
                                "\r\n\r\n";

    size_t headerSize = httpResponseHeader.size();

    char *result = new char[headerSize + dataSize ];

    std::memcpy(result, httpResponseHeader.c_str(), headerSize);
    std::memcpy(result + headerSize, data, dataSize);

    int socketResult =
        send(this->clientSocket, result, headerSize + dataSize, 0);
    if (socketResult == SOCKET_ERROR) {
      std::cerr << "Send failed: " << WSAGetLastError() << "\n";
      closesocket(clientSocket);
    } else {
      std::cout << "Sent " << (headerSize+ dataSize)<<" SentEnd.\n";
    }


    delete[] result;
  }

private:
};
