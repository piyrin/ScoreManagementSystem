#include "AdminController.h"
#include "../../utils/MD5Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>

AdminController::AdminController() : studentDao(), teacherDao() {}

UserModel AdminController::getUserFromRequest(const HttpRequest &req)
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

void AdminController::showDashboard(const HttpRequest &req, HttpResponse &res)
{
    UserModel user = getUserFromRequest(req);
    if (user.getId() == 0 || user.getRole() != UserRole::ADMIN)
    {
        res.setStatusCode(302);
        res.setHeader("Location", "/login");
        return;
    }

    std::string filePath = "res/view/admin/dashboard.html";
    std::ifstream file(filePath);
    if (file)
    {
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        res.setBody(content);
    }
    else
    {
        res.setStatusCode(404);
        res.setBody("未找到管理员仪表盘");
    }
}

void AdminController::listUsers(const HttpRequest &req, HttpResponse &res)
{
    UserModel user = getUserFromRequest(req);
    if (user.getId() == 0 || user.getRole() != UserRole::ADMIN)
    {
        res.setStatusCode(403);
        res.setBody("访问被拒绝");
        return;
    }

    auto users = userDao.selectAll();
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < users.size(); ++i)
    {
        std::string name = "";
        std::string no = "";
        std::string gender = "";

        if (users[i].getRole() == UserRole::STUDENT)
        {
            auto student = studentDao.selectById(users[i].getRelatedId());
            if (student.getId() != 0)
            {
                name = student.getName();
                no = student.getStudentNo();
                gender = student.getGender();
            }
        }
        else if (users[i].getRole() == UserRole::TEACHER)
        {
            auto teacher = teacherDao.selectById(users[i].getRelatedId());
            if (teacher.getId() != 0)
            {
                name = teacher.getName();
                no = teacher.getTeacherNo();
                gender = teacher.getGender();
            }
        }

        ss << "{\"id\": " << users[i].getId()
           << ", \"username\": \"" << users[i].getUsername() << "\""
           << ", \"role\": " << static_cast<int>(users[i].getRole())
           << ", \"relatedId\": " << users[i].getRelatedId()
           << ", \"name\": \"" << name << "\""
           << ", \"no\": \"" << no << "\""
           << ", \"gender\": \"" << gender << "\""
           << "}";
        if (i < users.size() - 1)
            ss << ",";
    }
    ss << "]";

    res.setContentType("application/json; charset=utf-8");
    res.setBody(ss.str());
}

void AdminController::addUser(const HttpRequest &req, HttpResponse &res)
{
    UserModel user = getUserFromRequest(req);
    if (user.getId() == 0 || user.getRole() != UserRole::ADMIN)
    {
        res.setStatusCode(403);
        res.setBody("访问被拒绝");
        return;
    }

    auto params = req.getPostParams();
    try
    {
        std::string username = params["username"];
        std::string password = params["password"];
        int role = std::stoi(params["role"]);
        std::string name = params["name"];
        std::string no = params["no"];
        std::string gender = params["gender"];

        AdminOpResult result = AdminOpResult::PARAM_ERROR;

        if (role == static_cast<int>(UserRole::TEACHER))
        {
            TeacherModel teacher(no, name, gender, "Lecturer", "CS", "");
            result = adminService.addTeacherUser(user, username, password, teacher);
        }
        else if (role == static_cast<int>(UserRole::STUDENT))
        {
            StudentModel student(no, name, gender, "CS", "2023", "");
            result = adminService.addStudentUser(user, username, password, student);
        }

        if (result == AdminOpResult::SUCCESS)
        {
            res.setBody("用户添加成功");
        }
        else
        {
            res.setStatusCode(400);
            res.setBody("添加用户失败");
        }
    }
    catch (...)
    {
        res.setStatusCode(400);
        res.setBody("参数无效");
    }
}

void AdminController::deleteUser(const HttpRequest &req, HttpResponse &res)
{
    UserModel user = getUserFromRequest(req);
    if (user.getId() == 0 || user.getRole() != UserRole::ADMIN)
    {
        res.setStatusCode(403);
        res.setBody("访问被拒绝");
        return;
    }

    auto params = req.getPostParams();
    try
    {
        int userId = std::stoi(params["userId"]);
        AdminOpResult result = adminService.deleteUser(user, userId);

        if (result == AdminOpResult::SUCCESS)
        {
            res.setBody("用户删除成功");
        }
        else
        {
            res.setStatusCode(400);
            res.setBody("删除用户失败");
        }
    }
    catch (...)
    {
        res.setStatusCode(400);
        res.setBody("参数无效");
    }
}

void AdminController::updateUser(const HttpRequest &req, HttpResponse &res)
{
    UserModel user = getUserFromRequest(req);
    if (user.getId() == 0 || user.getRole() != UserRole::ADMIN)
    {
        res.setStatusCode(403);
        res.setBody("访问被拒绝");
        return;
    }

    auto params = req.getPostParams();
    try
    {
        int userId = std::stoi(params["userId"]);
        UserModel targetUser = userDao.selectById(userId);
        if (targetUser.getId() == 0)
        {
            res.setStatusCode(404);
            res.setBody("未找到用户");
            return;
        }

        // Update User Account Info
        bool userChanged = false;
        if (params.count("username") && !params["username"].empty())
        {
            targetUser.setUsername(params["username"]);
            userChanged = true;
        }
        if (params.count("password") && !params["password"].empty())
        {
            targetUser.setPassword(MD5Utils::encrypt(params["password"]));
            userChanged = true;
        }

        if (userChanged)
        {
            userDao.update(targetUser);
        }

        AdminOpResult result = AdminOpResult::SUCCESS;

        if (targetUser.getRole() == UserRole::STUDENT)
        {
            StudentModel student = studentDao.selectById(targetUser.getRelatedId());
            if (student.getId() != 0)
            {
                if (params.count("name"))
                    student.setName(params["name"]);
                if (params.count("no"))
                    student.setStudentNo(params["no"]);
                if (params.count("gender"))
                    student.setGender(params["gender"]);

                result = adminService.updateStudent(user, student);
            }
        }
        else if (targetUser.getRole() == UserRole::TEACHER)
        {
            TeacherModel teacher = teacherDao.selectById(targetUser.getRelatedId());
            if (teacher.getId() != 0)
            {
                if (params.count("name"))
                    teacher.setName(params["name"]);
                if (params.count("no"))
                    teacher.setTeacherNo(params["no"]);
                if (params.count("gender"))
                    teacher.setGender(params["gender"]);

                result = adminService.updateTeacher(user, teacher);
            }
        }

        if (result == AdminOpResult::SUCCESS)
        {
            res.setBody("用户更新成功");
        }
        else if (result == AdminOpResult::DUPLICATE_DATA)
        {
            res.setStatusCode(400);
            res.setBody("数据重复 (学号/工号)");
        }
        else
        {
            res.setStatusCode(400);
            res.setBody("更新用户详情失败");
        }
    }
    catch (...)
    {
        res.setStatusCode(400);
        res.setBody("参数无效");
    }
}

void AdminController::addCourse(const HttpRequest &req, HttpResponse &res)
{
    UserModel user = getUserFromRequest(req);
    if (user.getId() == 0 || user.getRole() != UserRole::ADMIN)
    {
        res.setStatusCode(403);
        res.setBody("访问被拒绝");
        return;
    }

    auto params = req.getPostParams();
    try
    {
        CourseModel course;
        course.setCourseNo(params["courseNo"]);
        course.setCourseName(params["courseName"]);
        course.setCredit(std::stoi(params["credit"]));
        course.setTeacherId(std::stoi(params["teacherId"]));
        course.setDescription(params["description"]);

        AdminOpResult result = adminService.addCourse(user, course);

        if (result == AdminOpResult::SUCCESS)
        {
            res.setBody("课程添加成功");
        }
        else
        {
            res.setStatusCode(400);
            res.setBody("添加课程失败");
        }
    }
    catch (...)
    {
        res.setStatusCode(400);
        res.setBody("参数无效");
    }
}

void AdminController::deleteCourse(const HttpRequest &req, HttpResponse &res)
{
    UserModel user = getUserFromRequest(req);
    if (user.getId() == 0 || user.getRole() != UserRole::ADMIN)
    {
        res.setStatusCode(403);
        res.setBody("访问被拒绝");
        return;
    }

    auto params = req.getPostParams();
    try
    {
        int courseId = std::stoi(params["courseId"]);
        AdminOpResult result = adminService.deleteCourse(user, courseId);

        if (result == AdminOpResult::SUCCESS)
        {
            res.setBody("课程删除成功");
        }
        else
        {
            res.setStatusCode(400);
            res.setBody("删除课程失败");
        }
    }
    catch (...)
    {
        res.setStatusCode(400);
        res.setBody("参数无效");
    }
}

void AdminController::updateCourse(const HttpRequest &req, HttpResponse &res)
{
    UserModel user = getUserFromRequest(req);
    if (user.getId() == 0 || user.getRole() != UserRole::ADMIN)
    {
        res.setStatusCode(403);
        res.setBody("访问被拒绝");
        return;
    }

    auto params = req.getPostParams();
    try
    {
        int courseId = std::stoi(params["courseId"]);
        // 先获取原课程信息，再覆盖更新
        // 这里简化处理，假设前端传了所有必要信息，或者我们在Service层做合并
        // 但AdminService::updateCourse需要完整的CourseModel
        // 所以最好先查出来
        // 由于AdminController没有直接持有CourseDao，我们这里构造一个新的CourseModel
        // 注意：AdminService::updateCourse会检查ID是否存在

        CourseModel course;
        course.setId(courseId);
        course.setCourseNo(params["courseNo"]);
        course.setCourseName(params["courseName"]);
        course.setCredit(std::stoi(params["credit"]));
        course.setTeacherId(std::stoi(params["teacherId"]));
        course.setDescription(params["description"]);

        AdminOpResult result = adminService.updateCourse(user, course);

        if (result == AdminOpResult::SUCCESS)
        {
            res.setBody("课程更新成功");
        }
        else
        {
            res.setStatusCode(400);
            res.setBody("更新课程失败");
        }
    }
    catch (...)
    {
        res.setStatusCode(400);
        res.setBody("参数无效");
    }
}

void AdminController::listCourses(const HttpRequest &req, HttpResponse &res)
{
    UserModel user = getUserFromRequest(req);
    if (user.getId() == 0 || user.getRole() != UserRole::ADMIN)
    {
        res.setStatusCode(403);
        res.setBody("访问被拒绝");
        return;
    }

    auto courses = adminService.queryAllCourses(user);
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < courses.size(); ++i)
    {
        ss << "{\"id\": " << courses[i].getId()
           << ", \"courseNo\": \"" << courses[i].getCourseNo() << "\""
           << ", \"courseName\": \"" << courses[i].getCourseName() << "\""
           << ", \"credit\": " << courses[i].getCredit()
           << ", \"teacherId\": " << courses[i].getTeacherId()
           << ", \"description\": \"" << courses[i].getDescription() << "\""
           << "}";
        if (i < courses.size() - 1)
            ss << ",";
    }
    ss << "]";

    res.setContentType("application/json; charset=utf-8");
    res.setBody(ss.str());
}
