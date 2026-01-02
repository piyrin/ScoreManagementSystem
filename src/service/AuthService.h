#ifndef AUTH_SERVICE_H
#define AUTH_SERVICE_H

#include<string>
#include "../core/Model/UserModel.h"
#include "../core/Model/StudentModel.h"
#include "../core/Model/TeacherModel.h"

//登录结果枚举
enum class LoginResult
{
    SUCCESS,        // 登录成功
    USER_NOT_FOUND, // 用户名不存在
    PASSWORD_ERROR, // 密码错误
    SYSTEM_ERROR    // 系统错误（数据库异常）
};

class AuthService
{
public:
    AuthService();
    ~AuthService();

    //用户登录（用户名+密码）
    LoginResult login(const std::string &username, const std::string &password, UserModel &out_user);

    //密码重置（需验证原密码）
    bool resetPassword(const std::string &username, const std::string &old_pwd, const std::string &new_pwd);

private:
    class UserDao *user_dao;
    class StudentDao *student_dao;
    class TeacherDao *teacher_dao;
};

#endif