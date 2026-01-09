#include "UserController.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

UserController::UserController() {}

UserModel UserController::getUserFromRequest(const HttpRequest &req)
{
    std::string cookie = req.getHeader("Cookie");
    if (cookie.empty())
        return UserModel();

    size_t pos = cookie.find("user_id=");
    if (pos == std::string::npos)
        return UserModel();

    std::string userIdStr = cookie.substr(pos + 8);
    size_t endPos = userIdStr.find(';');
    if (endPos != std::string::npos)
    {
        userIdStr = userIdStr.substr(0, endPos);
    }

    try
    {
        int userId = std::stoi(userIdStr);
        return userDao.selectById(userId);
    }
    catch (...)
    {
        return UserModel();
    }
}

void UserController::showDashboard(const HttpRequest &req, HttpResponse &res)
{
    UserModel user = getUserFromRequest(req);
    if (user.getId() == 0)
    {
        res.setStatusCode(302);
        res.setHeader("Location", "/login");
        return;
    }

    std::string filePath;
    if (user.getRole() == UserRole::STUDENT)
    {
        filePath = "res/view/user/dashboard.html"; 
    }
    else if (user.getRole() == UserRole::TEACHER)
    {
        filePath = "res/view/teacher/dashboard.html";
    }
    else
    {
        res.setStatusCode(403);
        res.setBody("访问被拒绝");
        return;
    }

    std::ifstream file(filePath);
    if (file)
    {
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        if (user.getRole() == UserRole::STUDENT)
        {
            double gpa = studentService.calculateGPA(user);
            std::stringstream ss;
            ss << std::fixed << std::setprecision(2) << gpa;
            std::string gpaStr = ss.str();

            // 字符串替换
            size_t pos = content.find("{{GPA}}");
            if (pos != std::string::npos)
            {
                content.replace(pos, 7, gpaStr);
            }
        }

        res.setBody(content);
    }
    else
    {
        res.setStatusCode(404);
        res.setBody("未找到界面");
    }
}

void UserController::getStudentScores(const HttpRequest &req, HttpResponse &res)
{
    UserModel user = getUserFromRequest(req);
    if (user.getId() == 0 || user.getRole() != UserRole::STUDENT)
    {
        res.setStatusCode(403);
        res.setBody("访问被拒绝");
        return;
    }

    auto scores = studentService.getMyScore(user);
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < scores.size(); ++i)
    {
        CourseModel course = courseDao.selectById(scores[i].getCourseId());
        ss << "{\"courseId\": " << scores[i].getCourseId()
           << ", \"courseName\": \"" << course.getCourseName() << "\""
           << ", \"score\": " << scores[i].getScore() << "}";
        if (i < scores.size() - 1)
            ss << ",";
    }
    ss << "]";

    res.setContentType("application/json; charset=utf-8");
    res.setBody(ss.str());
}

void UserController::addScore(const HttpRequest &req, HttpResponse &res)
{
    UserModel user = getUserFromRequest(req);
    if (user.getId() == 0 || user.getRole() != UserRole::TEACHER)
    {
        res.setStatusCode(403);
        res.setBody("访问被拒绝");
        return;
    }

    auto params = req.getPostParams();
    try
    {
        std::string studentNo = params["studentNo"];
        int courseId = std::stoi(params["courseId"]);
        double scoreVal = std::stod(params["score"]);

        StudentModel student = studentDao.selectByStudentNo(studentNo);
        if (student.getId() == 0)
        {
            res.setStatusCode(400);
            res.setBody("未找到学号为 " + studentNo + " 的学生");
            return;
        }
        int studentId = student.getId();

        // 创建 ScoreModel (id=0, studentId, courseId, score, teacherId)
        ScoreModel score(0, studentId, courseId, scoreVal, user.getRelatedId());

        TeacherOpResult result = teacherService.addStudentScore(user, score);

        if (result == TeacherOpResult::SUCCESS)
        {
            res.setBody("成绩保存成功");
        }
        else if (result == TeacherOpResult::NO_PERMISSION)
        {
            res.setStatusCode(403);
            res.setBody("您未教授此课程");
        }
        else
        {
            res.setStatusCode(400);
            res.setBody("添加成绩失败");
        }
    }
    catch (...)
    {
        res.setStatusCode(400);
        res.setBody("参数无效");
    }
}

void UserController::getTeacherCourses(const HttpRequest &req, HttpResponse &res)
{
    UserModel user = getUserFromRequest(req);
    if (user.getId() == 0 || user.getRole() != UserRole::TEACHER)
    {
        res.setStatusCode(403);
        res.setBody("访问被拒绝");
        return;
    }

    auto courses = teacherService.getMyCourse(user);
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < courses.size(); ++i)
    {
        ss << "{\"id\": " << courses[i].getId()
           << ", \"courseNo\": \"" << courses[i].getCourseNo() << "\""
           << ", \"courseName\": \"" << courses[i].getCourseName() << "\"}";
        if (i < courses.size() - 1)
            ss << ",";
    }
    ss << "]";

    res.setContentType("application/json; charset=utf-8");
    res.setBody(ss.str());
}

void UserController::getCourseStats(const HttpRequest &req, HttpResponse &res)
{
    UserModel user = getUserFromRequest(req);
    if (user.getId() == 0 || user.getRole() != UserRole::TEACHER)
    {
        res.setStatusCode(403);
        res.setBody("访问被拒绝");
        return;
    }

    auto courses = teacherService.getMyCourse(user);
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < courses.size(); ++i)
    {
        auto scores = teacherService.getCourseScores(user, courses[i].getId());
        auto stats = teacherService.statCourseScore(scores);

        ss << "{\"courseName\": \"" << courses[i].getCourseName() << "\""
           << ", \"total\": " << scores.size()
           << ", \"avg\": " << stats.first
           << ", \"passRate\": " << stats.second << "}";
        if (i < courses.size() - 1)
            ss << ",";
    }
    ss << "]";

    res.setContentType("application/json; charset=utf-8");
    res.setBody(ss.str());
}
