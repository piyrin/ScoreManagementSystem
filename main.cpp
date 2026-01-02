#include <iostream>
#include "src/network/HttpServer.h"
#include <windows.h>
#include "src/core/Controller/AuthController.h"
#include "src/core/Controller/UserController.h"
#include "src/core/Controller/AdminController.h"
#include "src/config/Config.h"

int main()
{
    SetConsoleOutputCP(65001); // 设置控制台为 UTF-8

    Config *config = Config::getInstance();
    int port = config->getServerConfig().port;

    HttpServer server(port);

    AuthController authController;
    UserController userController;
    AdminController adminController;

    // 静态文件
    server.registerStaticFolder("/css", "res/public/css");
    server.registerStaticFolder("/js", "res/public/js");

    // 路由
    server.registerHandler("/", [&](const HttpRequest &req, HttpResponse &res)
                           {
        res.setStatusCode(302);
        res.setHeader("Location", "/login"); });

    server.registerHandler("/login", [&](const HttpRequest &req, HttpResponse &res)
                           {
        if (req.getMethod() == "GET") {
            authController.showLoginPage(req, res);
        } else if (req.getMethod() == "POST") {
            authController.login(req, res);
        } });

    // 学生/教师路由
    server.registerHandler("/student/dashboard", [&](const HttpRequest &req, HttpResponse &res)
                           { userController.showDashboard(req, res); });

    server.registerHandler("/teacher/dashboard", [&](const HttpRequest &req, HttpResponse &res)
                           { userController.showDashboard(req, res); });

    server.registerHandler("/student/scores", [&](const HttpRequest &req, HttpResponse &res)
                           { userController.getStudentScores(req, res); });

    server.registerHandler("/teacher/addScore", [&](const HttpRequest &req, HttpResponse &res)
                           { userController.addScore(req, res); });

    server.registerHandler("/teacher/courses", [&](const HttpRequest &req, HttpResponse &res)
                           { userController.getTeacherCourses(req, res); });

    server.registerHandler("/teacher/stats", [&](const HttpRequest &req, HttpResponse &res)
                           { userController.getCourseStats(req, res); });

    // 管理员路由
    server.registerHandler("/admin/dashboard", [&](const HttpRequest &req, HttpResponse &res)
                           { adminController.showDashboard(req, res); });

    server.registerHandler("/admin/users", [&](const HttpRequest &req, HttpResponse &res)
                           { adminController.listUsers(req, res); });

    server.registerHandler("/admin/addUser", [&](const HttpRequest &req, HttpResponse &res)
                           { adminController.addUser(req, res); });

    server.registerHandler("/admin/deleteUser", [&](const HttpRequest &req, HttpResponse &res)
                           { adminController.deleteUser(req, res); });

    server.registerHandler("/admin/updateUser", [&](const HttpRequest &req, HttpResponse &res)
                           { adminController.updateUser(req, res); });

    server.registerHandler("/admin/addCourse", [&](const HttpRequest &req, HttpResponse &res)
                           { adminController.addCourse(req, res); });

    server.registerHandler("/admin/deleteCourse", [&](const HttpRequest &req, HttpResponse &res)
                           { adminController.deleteCourse(req, res); });

    server.registerHandler("/admin/updateCourse", [&](const HttpRequest &req, HttpResponse &res)
                           { adminController.updateCourse(req, res); });

    server.registerHandler("/admin/courses", [&](const HttpRequest &req, HttpResponse &res)
                           { adminController.listCourses(req, res); });

    std::cout << "Starting server on port " << port << "..." << std::endl;
    std::cout << "Open http://127.0.0.1:" << port << " in your browser." << std::endl;

    server.start();

    return 0;
}
