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

class StudentService
{
public:
    StudentService();
    ~StudentService();
    
    //查询个人信息，通过登录后的UserModel获取relatedId=student.id
    StudentModel getStudentInfo(const UserModel &login_user);

    //修改个人信息
    StudentOpResult updateStudentInfo(const UserModel &login_user, const StudentModel &new_info);

    //查询个人所有成绩（返回该学生成绩列表）
    std::vector<ScoreModel> getMyScore(const UserModel &login_user);

private:
    class StudentDao *student_dao;
    class ScoreDao *score_dao;

};

#endif