#include "AuthController.h"
#include <iostream>
#include <fstream>
#include "../../config/Config.h"

AuthController::AuthController() {}

void AuthController::showLoginPage(const HttpRequest &req, HttpResponse &res)
{
    std::string filePath = "res/view/common/login.html";
    std::ifstream file(filePath);
    if (file)
    {
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        res.setBody(content);
    }
    else
    {
        res.setStatusCode(404);
        res.setBody("未找到登录页面");
    }
}

void AuthController::login(const HttpRequest &req, HttpResponse &res)
{
    auto params = req.getPostParams();
    std::string username = params["username"];
    std::string password = params["password"];

    UserModel user;
    LoginResult result = authService.login(username, password, user);

    if (result == LoginResult::SUCCESS)
    {
        // 根据角色重定向
        if (user.getRole() == UserRole::ADMIN)
        {
            res.setStatusCode(302);
            res.setHeader("Location", "/admin/dashboard");
        }
        else if (user.getRole() == UserRole::TEACHER)
        {
            res.setStatusCode(302);
            res.setHeader("Location", "/teacher/dashboard"); 
        }
        else if (user.getRole() == UserRole::STUDENT)
        {
            res.setStatusCode(302);
            res.setHeader("Location", "/student/dashboard"); 
        }
        res.setHeader("Set-Cookie", "user_id=" + std::to_string(user.getId()) + "; Path=/");
    }
    else
    {
        res.setStatusCode(401);
        res.setBody("登录失败");
    }
}
