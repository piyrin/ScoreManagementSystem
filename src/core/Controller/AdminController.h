#ifndef ADMINCONTROLLER_H
#define ADMINCONTROLLER_H

#include "../../network/HttpRequest.h"
#include "../../network/HttpResponse.h"
#include "../../service/AdminService.h"
#include "../../dao/UserDao.h"
#include "../../dao/StudentDao.h"
#include "../../dao/TeacherDao.h"

// 管理员控制器类，处理管理员相关的HTTP请求
class AdminController
{
public:
    AdminController();
    void showDashboard(const HttpRequest &req, HttpResponse &res);

    // 获取用户列表 
    void listUsers(const HttpRequest &req, HttpResponse &res);

    // 添加新用户
    void addUser(const HttpRequest &req, HttpResponse &res);

    // 删除用户
    void deleteUser(const HttpRequest &req, HttpResponse &res);

    // 更新用户信息
    void updateUser(const HttpRequest &req, HttpResponse &res);

    // 添加新课程
    void addCourse(const HttpRequest &req, HttpResponse &res);

    // 删除课程
    void deleteCourse(const HttpRequest &req, HttpResponse &res);

    // 更新课程信息
    void updateCourse(const HttpRequest &req, HttpResponse &res);

    // 获取课程列表 
    void listCourses(const HttpRequest &req, HttpResponse &res);

private:
    AdminService adminService; // 管理员业务服务
    UserDao userDao;           // 用户数据访问对象
    StudentDao studentDao;     // 学生数据访问对象
    TeacherDao teacherDao;     // 教师数据访问对象

    // 从请求的Cookie中获取当前登录用户
    UserModel getUserFromRequest(const HttpRequest &req);
};

#endif 
