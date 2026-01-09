#include "AuthService.h"
#include "../dao/UserDao.h"
#include "../dao/StudentDao.h"
#include "../dao/TeacherDao.h"
#include "../utils/MD5Utils.h"
#include <iostream>

AuthService::AuthService()
{
    user_dao = new UserDao();
    student_dao = new StudentDao();
    teacher_dao = new TeacherDao();
}

AuthService::~AuthService()
{
    delete user_dao;
    delete student_dao;
    delete teacher_dao;
}

// 登录：校验用户名+密码，返回用户信息
LoginResult AuthService::login(const std::string &username, const std::string &password, UserModel &out_user)
{
    // 查询用户
    UserModel db_user = user_dao->selectByUsername(username);
    if (db_user.getId() == 0)
    {
        std::cerr << "登录失败：用户名" << username << "不存在" << std::endl;
        return LoginResult::USER_NOT_FOUND;
    }

    // 对比密码
    std::string encrypted_pwd = MD5Utils::encrypt(password);
    if (encrypted_pwd != db_user.getPassword())
    {
        std::cerr << "登录失败：密码错误(用户名：" << username << ")" << std::endl;
        return LoginResult::PASSWORD_ERROR;
    }

    //登录成功，返回用户信息
    out_user = db_user;
    std::cout << "登录成功：" << out_user.toString() << std::endl;
    return LoginResult::SUCCESS;
}

// 密码重置
bool AuthService::resetPassword(const std::string &username, const std::string &old_pwd, const std::string &new_pwd)
{
    // 校验参数
    if (username.empty() || old_pwd.empty() || new_pwd.empty())
    {
        std::cerr << "密码重置失败：参数为空" << std::endl;
        return false;
    }

    //查询用户
    UserModel db_user = user_dao->selectByUsername(username);
    if (db_user.getId() == 0)
    {
        std::cerr << "密码重置失败：用户名" << username << "不存在" << std::endl;
        return false;
    }

    //验证原密码
    if (MD5Utils::encrypt(old_pwd) != db_user.getPassword())
    {
        std::cerr << "密码重置失败：原密码错误(用户名：" << username << ")" << std::endl;
        return false;
    }

    // 更新新密码
    bool result = user_dao->updatePassword(db_user.getId(), MD5Utils::encrypt(new_pwd));
    if (result)
    {
        std::cout << "密码重置成功：用户名" << username << std::endl;
    }
    else
    {
        std::cerr << "密码重置失败：数据库更新失败(用户名：" << username << ")" << std::endl;
    }
    return result;
}