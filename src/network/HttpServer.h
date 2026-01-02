#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <string>
#include <map>
#include <functional>
#include <winsock2.h>
#include "HttpRequest.h"
#include "HttpResponse.h"

// HTTP服务器类，用于监听端口并处理HTTP请求
class HttpServer
{
public:
    HttpServer(int port);
    ~HttpServer();

    // 启动服务器，开始监听
    void start();
    
    // 停止服务器
    void stop();

    // 请求处理函数的函数指针类型定义
    using Handler = std::function<void(const HttpRequest &, HttpResponse &)>;

    // 注册URL路径的处理函数
    // path: URL路径 (如 "/login")
    // handler: 处理函数
    void registerHandler(const std::string &path, Handler handler);
    
    // 注册静态文件目录
    // path: URL前缀 (如 "/static")
    // folderPath: 本地文件系统路径
    void registerStaticFolder(const std::string &path, const std::string &folderPath);

private:
    int port; // 监听端口
    SOCKET serverSocket; // 服务器Socket句柄
    bool isRunning; // 服务器运行状态
    std::map<std::string, Handler> handlers; // URL路由映射表
    std::map<std::string, std::string> staticFolders; // 静态文件目录映射表

    // 处理单个客户端连接
    void handleClient(SOCKET clientSocket);
    
    // 发送HTTP响应给客户端
    void sendResponse(SOCKET clientSocket, const HttpResponse &response);
    
    // 根据文件扩展名获取MIME类型
    std::string getMimeType(const std::string &filePath);
    
    // 处理静态文件请求
    void handleStaticFile(const HttpRequest &req, HttpResponse &res, const std::string &filePath);
};

#endif 
