/*
           /$$                                                 /$$      
          |__/                                                | $$      
  /$$$$$$$ /$$ /$$$$$$/$$$$   /$$$$$$  /$$  /$$  /$$  /$$$$$$ | $$$$$$$ 
 /$$_____/| $$| $$_  $$_  $$ /$$__  $$| $$ | $$ | $$ /$$__  $$| $$__  $$
|  $$$$$$ | $$| $$ \ $$ \ $$| $$  \ $$| $$ | $$ | $$| $$$$$$$$| $$  \ $$
 \____  $$| $$| $$ | $$ | $$| $$  | $$| $$ | $$ | $$| $$_____/| $$  | $$
 /$$$$$$$/| $$| $$ | $$ | $$| $$$$$$$/|  $$$$$/$$$$/|  $$$$$$$| $$$$$$$/
|_______/ |__/|__/ |__/ |__/| $$____/  \_____/\___/  \_______/|_______/.BETA
                            | $$                                        
                            | $$                                        
                            |__/                                        
*/

////////////////////////////////////////////////////////////////////////////////

//g++ -std=c++20 -o main.exe main.cpp -lws2_32




#include <iostream>
#include <string>
#include <fstream>
#include <winsock2.h>
#include <sstream>
#include <cstdlib>
#include <filesystem> 
using namespace std;
#pragma comment(lib, "Ws2_32.lib")

string interlace = "\n";

using namespace filesystem; 

string readFile(const string& filename) {
    ifstream file(filename, ios::binary);
    string content((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
    return content;
}

string getContentType(const string& filename) {
    if (filename.ends_with(".html")) {
        return "text/html";
    } else if (filename.ends_with(".jpg") || filename.ends_with(".jpeg")) {
        return "image/jpeg";
    } else if (filename.ends_with(".png")) {
        return "image/png";
    } else if (filename.ends_with(".gif")) {
        return "image/gif";
    } else {
        return "application/octet-stream";
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << interlace + "Error initializing Winsock" + interlace;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << interlace + "Error creating server socket" + interlace;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080); 

    if (bind(serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << interlace + "Error binding server socket" + interlace;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, 10) == SOCKET_ERROR) {
        cerr << interlace + "Failed to put server socket into listening mode" + interlace;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    while (true) {
        sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "Error accepting connection\n";
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        char buffer[1024] = {0};
        recv(clientSocket, buffer, 1024, 0);

        stringstream ss(buffer);
        string requestType, requestUri, httpVersion;
        ss >> requestType >> requestUri >> httpVersion;

        string requestedFile = "../html" + requestUri;

        if (requestUri == "/") {
            requestedFile = "../html/index.html";
        }

        if (!filesystem::exists(requestedFile)) {
            string notFoundResponse = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
            send(clientSocket, notFoundResponse.c_str(), notFoundResponse.length(), 0);
            closesocket(clientSocket);
            continue;
        }

        string fileContent = readFile(requestedFile);
        string contentType = getContentType(requestedFile);

        string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: " + contentType + "\r\nContent-Length: " + to_string(fileContent.length()) + "\r\n\r\n" + fileContent;
        send(clientSocket, httpResponse.c_str(), httpResponse.length(), 0);

        closesocket(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
