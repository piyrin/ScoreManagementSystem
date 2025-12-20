#include"StudentService.h"
#include "../dao/StudentDao.h"
#include "../dao/ScoreDao.h"
#include<iostream>

StudentService::StudentService()
{
    student_dao = new StudentDao();
    score_dao = new ScoreDao();
}

StudentService::~StudentService()
{
    delete student_dao;
    delete score_dao;
}

//查询个人信息
StudentModel StudentService::getStudentInfo(const UserModel &login_user)
{
    if(login_user.getRole()!=UserRole::STUDENT)
    {
        std::cerr << "学生信息查询失败：角色错误(当前角色：" << login_user.getRoleStr() << ")" << std::endl;
        return StudentModel();
    }

    int student_id = login_user.getRelatedId();
    if(student_id==0)
    {
        std::cerr << "学生信息查询失败:关联ID无效" << std::endl;
        return StudentModel();
    }

    StudentModel student = student_dao->selectById(student_id);
    if (student.getId() == 0)
    {
        std::cerr << "学生信息查询失败:未找到ID为" << student_id << "的学生" << std::endl;
    }
    else
    {
        std::cout << "学生信息查询成功：" << student.toString() << std::endl;
    }

    return student;
}

StudentOpResult StudentService::updateStudentInfo(const UserModel &login_user, const StudentModel &new_info)
{
    if(login_user.getRole()!=UserRole::STUDENT)
    {
        std::cerr << "修改个人信息失败：角色错误" << std::endl;
        return StudentOpResult::ROLE_ERROR;
    }

    if(new_info.getName().empty())
    {
        std::cerr << "修改个人信息失败：姓名不能为空" << std::endl;
        return StudentOpResult::PARAM_ERROR;
    }

    int student_id = login_user.getRelatedId();
    if(student_id==0)
    {
        std::cerr << "修改个人信息失败:关联ID无效" << std::endl;
        return StudentOpResult::SYSTEM_ERROR;
    }

    StudentModel old_student = student_dao->selectById(student_id);
    if(old_student.getId()==0)
    {
        std::cerr << "修改个人信息失败：未找到学生" << std::endl;
        return StudentOpResult::NOT_FOUND;
    }
    
    StudentModel update_student = old_student;
    update_student.setName(new_info.getName());
    update_student.setMajor(new_info.getMajor());
    update_student.setGrade(new_info.getGrade());
    update_student.setPhone(new_info.getPhone());
    
    if(student_dao->update(update_student))
    {
        std::cout << "修改个人信息成功：" << update_student.toString() << std::endl;
        return StudentOpResult::SUCCESS;
    }
    else
    {
        std::cerr << "修改个人信息失败：数据库更新异常" << std::endl;
        return StudentOpResult::SYSTEM_ERROR;
    }
}

//查询个人成绩
std::vector<ScoreModel> StudentService::getMyScore(const UserModel &login_user)
{
    if(login_user.getRole()!=UserRole::STUDENT)
    {
        std::cerr << "成绩查询失败：角色错误" << std::endl;
        return std::vector<ScoreModel>();
    }
    // 获取学生ID
    int student_id = login_user.getRelatedId();
    if (student_id == 0)
    {
        std::cerr << "成绩查询失败:关联ID无效" << std::endl;
        return std::vector<ScoreModel>();
    }

    std::vector<ScoreModel> scores = score_dao->selectByStudentId(student_id);
    if(scores.empty())
    {
        std::cout << "成绩查询结果：暂无成绩记录" << std::endl;
    }
    else
    {
        std::cout << "成绩查询成功：共" << scores.size() << "门课程成绩" << std::endl;
    }

    //打印成绩
    for(const auto&score:scores)
    {
        std::cout << "课程ID:" << score.getCourseId()
                  << "| 考试分数:" << score.getScore()
                  << "| 考试时间:" << score.getExamTime();
    }
    return scores;
}