#include "AuthService.h"
#include "../dao/UserDao.h"
#include "../dao/StudentDao.h"
#include "../dao/TeacherDao.h"
#include "../utils/MD5Utils.h"
#include <iostream>

AuthService::AuthService()
{
    //初始化Dao层对象
    user_dao = new UserDao();
    student_dao = new StudentDao();
    teacher_dao = new TeacherDao();
}

AuthService::~AuthService()
{
    // 释放 Dao 层对象
    delete user_dao;
    delete student_dao;
    delete teacher_dao;
}

//学生注册，先插学生表，再插用户表，关联student.id，角色=3
RegisterResult AuthService::studentRegister(const std::string &username, const std::string &password, const StudentModel &studentInfo)
{
    //参数校验，必填项非空
    if (username.empty() || password.empty() || studentInfo.getName().empty() || studentInfo.getStudentNo().empty())
    {
        std::cerr << "学生注册失败:必填项为空" << std::endl;
        return RegisterResult::PARAM_ERROR;
    }

    //检查用户名是否存在
    if(user_dao->selectByUsername(username).getId()!=0)
    {
        std::cerr << "学生注册失败：用户名" << username << "已存在" << std::endl;
        return RegisterResult::USERNAME_DUPLICATE;
    }
    //检查学号是否存在
    if (student_dao->selectByStudentNo(studentInfo.getStudentNo()).getId() != 0)
    {
        std::cerr << "学生注册失败：学号" << studentInfo.getStudentNo() << "已存在" << std::endl;
        return RegisterResult::IDENTITY_DUPLICATE;
    }
    //先插入学生信息，获取自增id
    if(!student_dao->insert(studentInfo))
    {
        std::cerr << "学生注册失败：插入学生信息失败" << std::endl;
        return RegisterResult::SYSTEM_ERROR;
    }
    //获取刚插入学生的id
    StudentModel new_student = student_dao->selectByStudentNo(studentInfo.getStudentNo());
    if (new_student.getId() == 0)
    {
        std::cerr << "学生注册失败:获取学生ID失败" << std::endl;
        return RegisterResult::SYSTEM_ERROR;
    }
    //插入用户表（密码，角色=学生（3），关联id=学生id）
    UserModel user(0, username, MD5Utils::encrypt(password), UserRole::STUDENT, new_student.getId());
    if(user_dao->insert(user))
    {
        std::cout << "学生注册成功：" << user.toString() << std::endl;
        return RegisterResult::SUCCESS;
    }
    else
    {
        // 回滚：用户表插入失败，删除已插入的学生信息
        student_dao->deleteById(new_student.getId());
        std::cerr << "学生注册失败：插入用户信息失败（已回滚学生数据）" << std::endl;
        return RegisterResult::SYSTEM_ERROR;
    }
}

//教师注册：先插教师表，在插用户表，关联teacher.id，角色=2
RegisterResult AuthService::teacherRegister(const std::string &username, const std::string &password, const TeacherModel &teacherInfo)
{
    //参数校验
    if (username.empty() || password.empty() || teacherInfo.getTeacherNo().empty() || teacherInfo.getName().empty())
    {
        std::cerr << "教师注册失败：必填参数为空" << std::endl;
        return RegisterResult::PARAM_ERROR;
    }
    //检查用户名是否存在
    if (user_dao->selectByUsername(username).getId() != 0)
    {
        std::cerr << "教师注册失败：用户名" << username << "已存在" << std::endl;
        return RegisterResult::USERNAME_DUPLICATE;
    }
    //检查工号是否存在
    if(teacher_dao->selectByTeacherNo(teacherInfo.getTeacherNo()).getId()!=0)
    {
        std::cerr << "教师注册失败：工号" << teacherInfo.getTeacherNo() << "已存在" << std::endl;
        return RegisterResult::IDENTITY_DUPLICATE;
    }

    //先插入教师信息
    if(!teacher_dao->insert(teacherInfo))
    {
        std::cerr << "教师注册失败：插入教师信息失败" << std::endl;
        return RegisterResult::SYSTEM_ERROR;
    }
    //获取刚插入教师的id
    TeacherModel new_teacher = teacher_dao->selectByTeacherNo(teacherInfo.getTeacherNo());
    if(new_teacher.getId()==0)
    {
        std::cerr << "教师注册失败:获取教师ID失败" << std::endl;
        return RegisterResult::SYSTEM_ERROR;
    }
    //插入用户表
    UserModel user(0, username, MD5Utils::encrypt(password), UserRole::TEACHER, new_teacher.getId());
    if (user_dao->insert(user))
    {
        std::cout << "教师注册成功：" << user.toString() << std::endl;
        return RegisterResult::SUCCESS;
    }
    else
    {
        // 回滚：删除教师信息
        teacher_dao->deleteById(new_teacher.getId());
        std::cerr << "教师注册失败：插入用户信息失败（已回滚教师数据）" << std::endl;
        return RegisterResult::SYSTEM_ERROR;
    }
}

// 管理员注册（角色=1，relatedId=0）
RegisterResult AuthService::adminRegister(const std::string &username, const std::string &password)
{
    // 参数校验
    if (username.empty() || password.empty())
    {
        std::cerr << "管理员注册失败：用户名/密码为空" << std::endl;
        return RegisterResult::PARAM_ERROR;
    }
    //检验用户名是否存在
    if(user_dao->selectByUsername(username).getId()!=0)
    {
        std::cerr << "管理员注册失败：用户名" << username << "已存在" << std::endl;
        return RegisterResult::USERNAME_DUPLICATE;
    }
    //插入管理员用户
    UserModel admin(0, username, MD5Utils::encrypt(password), UserRole::ADMIN, 0);
    if (user_dao->insert(admin))
    {
        std::cout << "管理员注册成功：" << admin.toString() << std::endl;
        return RegisterResult::SUCCESS;
    }
    else
    {
        std::cerr << "管理员注册失败：插入用户信息失败" << std::endl;
        return RegisterResult::SYSTEM_ERROR;
    }
}

// 登录：校验用户名+密码，返回用户信息
LoginResult AuthService::login(const std::string &username, const std::string &password, UserModel &out_user)
{
    // 1. 查询用户（通过用户名）
    UserModel db_user = user_dao->selectByUsername(username);
    if (db_user.getId() == 0)
    {
        std::cerr << "登录失败：用户名" << username << "不存在" << std::endl;
        return LoginResult::USER_NOT_FOUND;
    }

    // 2. 对比密码（明文加密后 vs 数据库存储的加密串）
    std::string encrypted_pwd = MD5Utils::encrypt(password);
    if (encrypted_pwd != db_user.getPassword())
    {
        std::cerr << "登录失败：密码错误(用户名：" << username << ")" << std::endl;
        return LoginResult::PASSWORD_ERROR;
    }

    // 3. 登录成功，返回用户信息
    out_user = db_user;
    std::cout << "登录成功：" << out_user.toString() << std::endl;
    return LoginResult::SUCCESS;
}

// 密码重置（验证原密码后更新）
bool AuthService::resetPassword(const std::string &username, const std::string &old_pwd, const std::string &new_pwd)
{
    // 1. 校验参数
    if (username.empty() || old_pwd.empty() || new_pwd.empty())
    {
        std::cerr << "密码重置失败：参数为空" << std::endl;
        return false;
    }

    // 2. 查询用户
    UserModel db_user = user_dao->selectByUsername(username);
    if (db_user.getId() == 0)
    {
        std::cerr << "密码重置失败：用户名" << username << "不存在" << std::endl;
        return false;
    }

    // 3. 验证原密码
    if (MD5Utils::encrypt(old_pwd) != db_user.getPassword())
    {
        std::cerr << "密码重置失败：原密码错误(用户名：" << username << ")" << std::endl;
        return false;
    }

    // 4. 更新新密码（加密后）
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