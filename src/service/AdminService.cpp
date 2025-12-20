#include "AdminService.h"
#include "../dao/UserDao.h"
#include "../dao/StudentDao.h"
#include "../dao/TeacherDao.h"
#include "../dao/CourseDao.h"
#include "../dao/ScoreDao.h"
#include "../utils/MD5Utils.h"
#include <iostream>
#include <iomanip>

AdminService::AdminService()
{
    user_dao = new UserDao();
    student_dao = new StudentDao();
    teacher_dao = new TeacherDao();
    course_dao = new CourseDao();
    score_dao = new ScoreDao();
}

AdminService::~AdminService()
{
    delete user_dao;
    delete student_dao;
    delete teacher_dao;
    delete course_dao;
    delete score_dao;
}

bool AdminService::checkAdminRole(const UserModel &login_admin)
{
    if(login_admin.getRole()!=UserRole::ADMIN)
    {
        std::cerr << "角色错误:当前角色非管理员(角色:"<<login_admin.getRoleStr() <<")"<< std::endl;
        return false;
    }
    return true;
}

bool AdminService::clearCourseRelatedScores(int courseId)
{
    std::vector<ScoreModel> scores = score_dao->selectByCourseId(courseId);
    if(scores.empty())
    {
        return true;
    }

    bool allDelete = true;
    for(const auto score:scores)
    {
        if(!score_dao->deleteById(score.getId()))
        {
            std::cerr << "清除成绩失败:成绩Id:" << score.getId() << std::endl;
            return false;
        }
    }
    return allDelete;
}

bool AdminService::clearUserRelatedData(int userId, UserRole role)
{
    UserModel user = user_dao->selectById(userID);
    if(user.getId()==0)
    {
        return false;
    }

    int relatedId = user.getRelatedId();
    bool success = true;

    switch(role)
    {
        case UserRole::STUDENT:
            std::vector<ScoreModel> student_scores = score_dao->selectByStudentId(relatedId);
            for (const auto &score : student_scores)
            {
                if (!score_dao->deleteById(score.getId()))
                {
                    std::cerr << "清理学生成绩失败:成绩ID=" << score.getId() << std::endl;
                    success = false;
                }
            }
            break;

        case UserRole::TEACHER:
            std::vector<CourseModel> teacher_courses = course_dao->selectByTeacherId(relatedId);
            for (const auto &course : teacher_courses)
            {
                if (!clearCourseRelatedScores(course.getId()))
                {
                    success = false;
                }
                if (!course_dao->deleteById(course.getId()))
                {
                    std::cerr << "清理教师授课课程失败:课程ID=" << course.getId() << std::endl;
                    success = false;
                }
            }
            if (!teacher_dao->deleteById(relatedId))
            {
                std::cerr << "清理教师信息失败:教师ID=" << relatedId << std::endl;
                success = false;
            }
            break;
        case UserRole::ADMIN:
            break;
        default:
            success = false;
            break;
    }
    return success;
}