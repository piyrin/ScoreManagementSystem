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

// 注册结果枚举 
enum class RegisterResult 
{
    SUCCESS,            // 注册成功
    USERNAME_DUPLICATE, // 用户名已存在
    IDENTITY_DUPLICATE, // 学号/工号已存在
    PARAM_ERROR,        // 参数错误（必填项为空）
    SYSTEM_ERROR        // 系统错误
};

class AuthService
{
public:
    AuthService();
    ~AuthService();

    //学生注册(用户名+密码+学生信息)
    RegisterResult studentRegister(const std::string &username, const std::string &password, const StudentModel &studentInfo);
    //教师注册(用户名+密码+教师信息)
    RegisterResult teacherRegister(const std::string &username, const std::string &password, const TeacherModel &teacherInfo);
    //管理员注册（仅用于初始化，后续可关闭该接口）
    RegisterResult adminRegister(const std::string &username, const std::string &password);

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