#include "HttpResponse.h"
#include <sstream>

HttpResponse::HttpResponse() : statusCode(200)
{
    headers["Content-Type"] = "text/html; charset=utf-8";
    headers["Connection"] = "close";
}

HttpResponse::~HttpResponse() {}

void HttpResponse::setStatusCode(int code)
{
    statusCode = code;
}

void HttpResponse::setHeader(const std::string &key, const std::string &value)
{
    headers[key] = value;
}

void HttpResponse::setBody(const std::string &body)
{
    this->body = body;
    headers["Content-Length"] = std::to_string(body.length());
}

void HttpResponse::setContentType(const std::string &type)
{
    headers["Content-Type"] = type;
}

std::string HttpResponse::toString() const
{
    std::stringstream ss;
    ss << "HTTP/1.1 " << statusCode << " " << getStatusMessage(statusCode) << "\r\n";
    for (const auto &header : headers)
    {
        ss << header.first << ": " << header.second << "\r\n";
    }
    ss << "\r\n";
    ss << body;
    return ss.str();
}

std::string HttpResponse::getStatusMessage(int code) const
{
    switch (code)
    {
    case 200:
        return "OK";
    case 302:
        return "Found";
    case 400:
        return "Bad Request";
    case 401:
        return "Unauthorized";
    case 403:
        return "Forbidden";
    case 404:
        return "Not Found";
    case 500:
        return "Internal Server Error";
    default:
        return "Unknown";
    }
}
