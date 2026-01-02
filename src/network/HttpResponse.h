#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include <map>

// HTTP响应类，用于构建和发送HTTP响应
class HttpResponse
{
public:
    HttpResponse();
    ~HttpResponse();

    // 设置HTTP状态码 (如 200, 404, 500)
    void setStatusCode(int code);
    
    // 设置HTTP响应头
    // key: 头部字段名
    // value: 头部字段值
    void setHeader(const std::string &key, const std::string &value);
    
    // 设置响应体内容
    void setBody(const std::string &body);
    
    // 设置Content-Type响应头
    void setContentType(const std::string &type);

    // 将响应对象转换为HTTP协议格式的字符串
    std::string toString() const;

private:
    int statusCode; // 状态码
    std::map<std::string, std::string> headers; // 响应头集合
    std::string body; // 响应体

    // 根据状态码获取对应的状态消息 (如 200 -> OK)
    std::string getStatusMessage(int code) const;
};

#endif 
