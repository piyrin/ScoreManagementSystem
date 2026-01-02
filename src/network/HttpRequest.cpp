#include "HttpRequest.h"
#include <sstream>
#include <iostream>
#include <algorithm>

HttpRequest::HttpRequest() {}

HttpRequest::~HttpRequest() {}

void HttpRequest::parse(const std::string &rawData)
{
    // 查找头部的结束位置
    size_t headerEnd = rawData.find("\r\n\r\n");
    if (headerEnd == std::string::npos)
    {
        headerEnd = rawData.find("\n\n");
    }

    std::string headerPart;
    if (headerEnd != std::string::npos)
    {
        headerPart = rawData.substr(0, headerEnd);
        body = rawData.substr(headerEnd + 4); // 跳过 \r\n\r\n
    }
    else
    {
        headerPart = rawData;
        body = "";
    }

    std::vector<std::string> lines = split(headerPart, "\n");
    if (!lines.empty())
    {
        parseRequestLine(trim(lines[0]));
        std::vector<std::string> headerLines;
        for (size_t i = 1; i < lines.size(); ++i)
        {
            if (!trim(lines[i]).empty())
            {
                headerLines.push_back(trim(lines[i]));
            }
        }
        parseHeaders(headerLines);
    }

    if (method == "POST")
    {
        parsePostParams(body);
    }
}

void HttpRequest::parseRequestLine(const std::string &line)
{
    std::vector<std::string> parts = split(line, " ");
    if (parts.size() >= 2)
    {
        method = parts[0];
        std::string fullPath = parts[1];

        size_t queryPos = fullPath.find('?');
        if (queryPos != std::string::npos)
        {
            path = fullPath.substr(0, queryPos);
            parseQueryString(fullPath.substr(queryPos + 1));
        }
        else
        {
            path = fullPath;
        }
    }
}

void HttpRequest::parseHeaders(const std::vector<std::string> &headerLines)
{
    for (const auto &line : headerLines)
    {
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos)
        {
            std::string key = trim(line.substr(0, colonPos));
            std::string value = trim(line.substr(colonPos + 1));
            headers[key] = value;
        }
    }
}

void HttpRequest::parseQueryString(const std::string &queryString)
{
    std::vector<std::string> pairs = split(queryString, "&");
    for (const auto &pair : pairs)
    {
        size_t eqPos = pair.find('=');
        if (eqPos != std::string::npos)
        {
            std::string key = pair.substr(0, eqPos);
            std::string value = pair.substr(eqPos + 1);
            queryParams[urlDecode(key)] = urlDecode(value);
        }
    }
}

void HttpRequest::parsePostParams(const std::string &bodyData)
{
    // 简单解析 application/x-www-form-urlencoded
    // 对于 multipart/form-data 或 json，需要更复杂的解析
    // 暂时假设为 urlencoded，因为它对于简单表单很常见
    std::vector<std::string> pairs = split(bodyData, "&");
    for (const auto &pair : pairs)
    {
        size_t eqPos = pair.find('=');
        if (eqPos != std::string::npos)
        {
            std::string key = pair.substr(0, eqPos);
            std::string value = pair.substr(eqPos + 1);
            postParams[urlDecode(key)] = urlDecode(value);
        }
    }
}

std::string HttpRequest::getMethod() const { return method; }
std::string HttpRequest::getPath() const { return path; }
std::string HttpRequest::getHeader(const std::string &key) const
{
    auto it = headers.find(key);
    return (it != headers.end()) ? it->second : "";
}
std::string HttpRequest::getBody() const { return body; }
std::string HttpRequest::getQueryParam(const std::string &key) const
{
    auto it = queryParams.find(key);
    return (it != queryParams.end()) ? it->second : "";
}
std::map<std::string, std::string> HttpRequest::getPostParams() const { return postParams; }

std::vector<std::string> HttpRequest::split(const std::string &s, const std::string &delimiter)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = s.find(delimiter);
    while (end != std::string::npos)
    {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
    tokens.push_back(s.substr(start));
    return tokens;
}

std::string HttpRequest::trim(const std::string &s)
{
    size_t first = s.find_first_not_of(" \t\r\n");
    if (std::string::npos == first)
    {
        return s;
    }
    size_t last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, (last - first + 1));
}

std::string HttpRequest::urlDecode(const std::string &str)
{
    std::string ret;
    char ch;
    int i, ii, len = str.length();

    for (i = 0; i < len; i++)
    {
        if (str[i] != '%')
        {
            if (str[i] == '+')
                ret += ' ';
            else
                ret += str[i];
        }
        else
        {
            sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            ret += ch;
            i = i + 2;
        }
    }
    return ret;
}
