#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include "../../network/HttpRequest.h"
#include "../../network/HttpResponse.h"
#include "../../service/AuthService.h"

// 认证控制器类，处理登录等认证相关的HTTP请求
class AuthController
{
public:
    AuthController();
    void login(const HttpRequest &req, HttpResponse &res);
    
    // 显示登录页面 (GET)
    void showLoginPage(const HttpRequest &req, HttpResponse &res);

private:
    AuthService authService; // 认证业务服务
};

#endif
