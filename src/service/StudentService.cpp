#include "StudentService.h"
#include "../dao/StudentDao.h"
#include "../dao/ScoreDao.h"
#include "../dao/CourseDao.h"
#include <iostream>
#include <cmath>

StudentService::StudentService()
{
    student_dao = new StudentDao();
    score_dao = new ScoreDao();
    course_dao = new CourseDao();
}

StudentService::~StudentService()
{
    delete student_dao;
    delete score_dao;
    delete course_dao;
}

// 查询个人信息
StudentModel StudentService::getStudentInfo(const UserModel &login_user)
{
    if (login_user.getRole() != UserRole::STUDENT)
    {
        std::cerr << "学生信息查询失败：角色错误(当前角色：" << login_user.getRoleStr() << ")" << std::endl;
        return StudentModel();
    }

    int student_id = login_user.getRelatedId();
    if (student_id == 0)
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

// 查询个人成绩
std::vector<ScoreModel> StudentService::getMyScore(const UserModel &login_user)
{
    if (login_user.getRole() != UserRole::STUDENT)
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
    if (scores.empty())
    {
        std::cout << "成绩查询结果：暂无成绩记录" << std::endl;
    }
    else
    {
        std::cout << "成绩查询成功：共" << scores.size() << "门课程成绩" << std::endl;
    }

    // 打印成绩
    for (const auto &score : scores)
    {
        std::cout << "课程ID:" << score.getCourseId()
                  << "| 考试分数:" << score.getScore();
    }
    return scores;
}

double StudentService::calculateGPA(const UserModel &login_user)
{
    if (login_user.getRole() != UserRole::STUDENT)
    {
        return 0.0;
    }

    int student_id = login_user.getRelatedId();
    if (student_id == 0)
    {
        return 0.0;
    }

    std::vector<ScoreModel> scores = score_dao->selectByStudentId(student_id);
    if (scores.empty())
    {
        return 0.0;
    }

    double total_weighted_gpa = 0.0;
    double total_credits = 0.0;

    for (const auto &score : scores)
    {
        CourseModel course = course_dao->selectById(score.getCourseId());
        if (course.getId() != 0)
        { // 确保课程存在
            double gpa = score.getGpa();
            double credit = course.getCredit();

            total_weighted_gpa += gpa * credit;
            total_credits += credit;
        }
    }

    if (total_credits == 0.0)
    {
        return 0.0;
    }

    // 保留两位小数
    double gpa = total_weighted_gpa / total_credits;
    return std::round(gpa * 100.0) / 100.0;
}