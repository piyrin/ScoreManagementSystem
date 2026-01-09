#include "HttpServer.h"
#include <iostream>
#include <fstream>
#include <ws2tcpip.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

HttpServer::HttpServer(int port) : port(port), serverSocket(INVALID_SOCKET), isRunning(false)
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        std::cerr << "WSAStartup 初始化失败: " << result << std::endl;
    }
}

HttpServer::~HttpServer()
{
    stop();
    WSACleanup();
}

void HttpServer::start()
{
    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    std::string portStr = std::to_string(port);
    if (getaddrinfo(NULL, portStr.c_str(), &hints, &result) != 0)
    {
        std::cerr << "获取地址信息失败" << std::endl;
        return;
    }

    serverSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cerr << "创建套接字失败: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        return;
    }

    if (bind(serverSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
    {
        std::cerr << "绑定端口失败，错误代码: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        closesocket(serverSocket);
        return;
    }

    freeaddrinfo(result);

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "监听失败，错误代码: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        return;
    }

    isRunning = true;
    std::cout << "服务器启动成功，监听端口: " << port << std::endl;

    while (isRunning)
    {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "接受连接失败: " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            break;
        }

        // 在单独的线程中处理客户端，以允许并发连接
        std::thread(&HttpServer::handleClient, this, clientSocket).detach();
    }
}

void HttpServer::stop()
{
    isRunning = false;
    if (serverSocket != INVALID_SOCKET)
    {
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
    }
}

void HttpServer::registerHandler(const std::string &path, Handler handler)
{
    handlers[path] = handler;
}

void HttpServer::registerStaticFolder(const std::string &path, const std::string &folderPath)
{
    staticFolders[path] = folderPath;
}

void HttpServer::handleClient(SOCKET clientSocket)
{
    char buffer[4096];
    int bytesReceived = recv(clientSocket, buffer, 4096, 0);
    if (bytesReceived > 0)
    {
        std::string rawRequest(buffer, bytesReceived);
        HttpRequest req;
        req.parse(rawRequest);

        HttpResponse res;
        std::string path = req.getPath();

        // 首先检查静态文件
        bool handled = false;
        for (const auto &pair : staticFolders)
        {
            if (path.find(pair.first) == 0)
            {
                std::string filePath = pair.second + path.substr(pair.first.length());
                handleStaticFile(req, res, filePath);
                handled = true;
                break;
            }
        }

        if (!handled)
        {
            if (handlers.find(path) != handlers.end())
            {
                handlers[path](req, res);
            }
            else
            {
                res.setStatusCode(404);
                res.setBody("<h1>404 未找到页面</h1>");
            }
        }

        sendResponse(clientSocket, res);
    }
    closesocket(clientSocket);
}

void HttpServer::sendResponse(SOCKET clientSocket, const HttpResponse &response)
{
    std::string responseStr = response.toString();
    send(clientSocket, responseStr.c_str(), responseStr.length(), 0);
}

void HttpServer::handleStaticFile(const HttpRequest &req, HttpResponse &res, const std::string &filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    if (file)
    {
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        res.setBody(content);
        res.setContentType(getMimeType(filePath));
    }
    else
    {
        res.setStatusCode(404);
        res.setBody("<h1>文件未找到</h1>");
    }
}

std::string HttpServer::getMimeType(const std::string &filePath)
{
    if (filePath.find(".html") != std::string::npos)
        return "text/html";
    if (filePath.find(".css") != std::string::npos)
        return "text/css";
    if (filePath.find(".js") != std::string::npos)
        return "application/javascript";
    if (filePath.find(".png") != std::string::npos)
        return "image/png";
    if (filePath.find(".jpg") != std::string::npos)
        return "image/jpeg";
    return "text/plain";
}
