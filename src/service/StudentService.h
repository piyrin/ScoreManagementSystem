#ifndef STUDENT_SERVICE_H
#define STUDENT_SERVICE_H

#include<vector>
#include "../core/Model/UserModel.h"
#include "../core/Model/StudentModel.h"
#include "../core/Model/ScoreModel.h"

// 学生业务操作结果枚举
enum class StudentOpResult
{
    SUCCESS,     // 操作成功
    ROLE_ERROR,  // 角色错误（非学生）
    PARAM_ERROR, // 参数错误（必填项为空）
    NOT_FOUND,   // 未找到学生信息/成绩
    SYSTEM_ERROR // 系统错误（数据库异常）
};

/**
 * 学生业务服务
 * 核心功能：
 * 1. 查询个人信息（仅当前登录学生）
 * 2. 修改个人信息（仅允许修改姓名、专业、年级、电话）
 * 3. 查询个人所有成绩（按课程分类展示）
 */
class StudentService
{
public:
    StudentService();
    ~StudentService();
    
    //查询个人信息，通过登录后的UserModel获取relatedId=student.id
    //参数：login_user=登录成功的用户对象，须为学生
    //返回：查询到的学生信息，id=0表示查询失败
    StudentModel getStudentInfo(const UserModel &login_user);

    //修改个人信息
    //参数：login_user=登录用户，new_info=新的学生信息
    //返回：StudentOpResult枚举（操作结果）
    StudentOpResult updateStudentInfo(const UserModel &login_user, const StudentModel &new_info);

    //查询个人所有成绩（返回该学生成绩列表）
    //参数:login_user=登录用户（学生角色）
    //返回：成绩列表（空列表表示查询失败/无成绩）
    std::vector<ScoreModel> getMyscore(const UserModel &login_user);

private:
    // 依赖Dao层对象（业务层不直接操作数据库）
    class StudentDao *student_dao;
    class ScoreDao *score_dao;
    
};

#endif