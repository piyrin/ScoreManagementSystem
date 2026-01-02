#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <map>
#include <vector>

// HTTP请求类，用于解析和存储HTTP请求信息
class HttpRequest
{
public:
    HttpRequest();
    ~HttpRequest();

    // 解析原始HTTP请求数据
    // rawData: 从socket接收到的原始请求字符串
    void parse(const std::string &rawData);

    // 获取HTTP请求方法 (GET, POST等)
    std::string getMethod() const;

    // 获取请求路径 (URL)
    std::string getPath() const;

    // 根据键获取HTTP头部信息
    // key: 头部字段名
    std::string getHeader(const std::string &key) const;

    // 获取请求体内容
    std::string getBody() const;

    // 获取URL查询参数
    std::string getQueryParam(const std::string &key) const;

    // 获取POST请求的表单参数
    std::map<std::string, std::string> getPostParams() const;

private:
    std::string method;                             // 请求方法
    std::string path;                               // 请求路径
    std::map<std::string, std::string> headers;     // 请求头集合
    std::map<std::string, std::string> queryParams; // URL查询参数集合
    std::string body;                               // 请求体
    std::map<std::string, std::string> postParams;  // POST表单参数集合

    // 解析请求行
    void parseRequestLine(const std::string &line);

    // 解析请求头
    void parseHeaders(const std::vector<std::string> &headerLines);

    // 解析URL查询字符串
    void parseQueryString(const std::string &queryString);

    // 解析POST表单数据
    void parsePostParams(const std::string &bodyData);

    // 字符串分割辅助函数
    std::vector<std::string> split(const std::string &s, const std::string &delimiter);

    // 字符串去空白辅助函数
    std::string trim(const std::string &s);

    // URL解码辅助函数
    std::string urlDecode(const std::string &str);
};

#endif
