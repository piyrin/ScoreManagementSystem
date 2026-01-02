#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include "../../network/HttpRequest.h"
#include "../../network/HttpResponse.h"
#include "../../service/StudentService.h"
#include "../../service/TeacherService.h"
#include "../../dao/UserDao.h"
#include "../../dao/StudentDao.h"
#include "../../dao/CourseDao.h"

// 用户控制器类，处理普通用户(学生/教师)相关的HTTP请求
class UserController
{
public:
    UserController();

    // 显示用户仪表盘页面
    void showDashboard(const HttpRequest &req, HttpResponse &res);

    // 获取学生成绩列表
    void getStudentScores(const HttpRequest &req, HttpResponse &res);

    // 录入成绩 (教师功能)
    void addScore(const HttpRequest &req, HttpResponse &res);

    // 获取教师授课列表
    void getTeacherCourses(const HttpRequest &req, HttpResponse &res);

    // 获取课程统计信息
    void getCourseStats(const HttpRequest &req, HttpResponse &res);

private:
    StudentService studentService; // 学生业务服务
    TeacherService teacherService; // 教师业务服务
    UserDao userDao;               // 用户数据访问对象
    StudentDao studentDao;         // 学生数据访问对象
    CourseDao courseDao;           // 课程数据访问对象

    // 获取当前登录用户
    UserModel getUserFromRequest(const HttpRequest &req);
};

#endif
