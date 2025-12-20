#ifndef TEACHER_SERVICE_H
#define TEACHER_SERVICE_H

#include <vector>
#include <string>
#include "../core/Model/UserModel.h"
#include "../core/Model/TeacherModel.h"
#include "../core/Model/CourseModel.h"
#include "../core/Model/ScoreModel.h"
#include "../core/Model/StudentModel.h"

class TeacherDao;
class CourseDao;
class StudentDao;
class ScoreDao;

// 教师业务操作结果枚举
enum class TeacherOpResult
{
    SUCCESS,         // 操作成功
    ROLE_ERROR,      // 角色错误（非教师）
    PARAM_ERROR,     // 参数错误（必填项为空/无效）
    NOT_FOUND,       // 未找到教师/课程/成绩信息
    NO_PERMISSION,   // 无权限（课程非本人授课）
    DUPLICATE_SCORE, // 成绩重复（学生+课程已存在成绩）
    SYSTEM_ERROR     // 系统错误（数据库异常）
};


class TeacherService
{
public:
    TeacherService();
    ~TeacherService();

    //查询个人信息，通过用户登录的relatedId=teacher.id
    TeacherModel getTeacherInfo(const UserModel &login_user);

    //修改个人信息
    TeacherOpResult updateTeacherInfo(const UserModel &login_user, const TeacherModel &new_info);

    //查询所教授课程列表，关联course的teacherId
    std::vector<CourseModel> getMyCourse(const UserModel &login_user);

    //录入学生成绩
    TeacherOpResult addStudentScore(const UserModel &login_user, const ScoreModel &score);

    //修改学生成绩
    TeacherOpResult updateStudentScore(const UserModel &login_user, const ScoreModel &score);

    //查询学生成绩：姓名，学号，成绩
    std::vector<std::pair<StudentModel, ScoreModel>> getCourseScores(const UserModel &login_user, int couresId);
    
    //课程成绩统计（平均分+及格率）
    std::pair<double, double> statCourseScore(const std::vector<std::pair<StudentModel, ScoreModel>> &courseScores);

private:
    TeacherDao *teacher_dao;
    CourseDao *course_dao;
    StudentDao *student_dao;
    ScoreDao *score_dao;
    // 辅助方法：校验课程是否为当前教师授课（内部调用，简化代码）
    bool checkCoursePermission(const UserModel &login_user, int courseId);
};

#endif
