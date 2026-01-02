#ifndef ADMIN_SERVICE_H
#define ADMIN_SERVICE_H

#include <vector>
#include <string>
#include "../core/Model/UserModel.h"
#include "../core/Model/StudentModel.h"
#include "../core/Model/TeacherModel.h"
#include "../core/Model/CourseModel.h"
#include "../core/Model/ScoreModel.h"

// 管理员业务操作结果枚举
enum class AdminOpResult
{
    SUCCESS,        // 操作成功
    ROLE_ERROR,     // 角色错误（非管理员）
    PARAM_ERROR,    // 参数错误（必填项为空/无效）
    NOT_FOUND,      // 未找到用户/课程/成绩
    DUPLICATE_DATA, // 数据重复（用户名/学号/工号/课程号已存在）
    SYSTEM_ERROR    // 系统错误
};

// 功能：用户管理
//      课程管理
//      统计数据
//      成绩管理

class AdminService
{
public:
    AdminService();
    ~AdminService();

    // 用户管理
    // 新增学生
    AdminOpResult addStudentUser(const UserModel &login_admin, const std::string &username, const std::string &password, const StudentModel &studentInfo);
    // 新增教师
    AdminOpResult addTeacherUser(const UserModel &login_admin, const std::string &username, const std::string &password, const TeacherModel &teacherInfo);
    // 删除用户
    AdminOpResult deleteUser(const UserModel &login_admin, int userId);

    // 修改学生信息
    AdminOpResult updateStudent(const UserModel &login_admin, const StudentModel &student);

    // 修改教师信息
    AdminOpResult updateTeacher(const UserModel &login_admin, const TeacherModel &teacher);

    // 课程管理
    // 新增课程
    AdminOpResult addCourse(const UserModel &login_admin, const CourseModel &course);

    // 修改课程
    AdminOpResult updateCourse(const UserModel &login_admin, const CourseModel &course);

    // 删除课程
    AdminOpResult deleteCourse(const UserModel &login_admin, int courseId);

    // 查询所有课程
    std::vector<CourseModel> queryAllCourses(const UserModel &login_admin);

private:
    class UserDao *user_dao;
    class StudentDao *student_dao;
    class TeacherDao *teacher_dao;
    class CourseDao *course_dao;
    class ScoreDao *score_dao;

    // 校验管理员角色
    bool checkAdminRole(const UserModel &login_admin);

    // 清理课程关联成绩
    bool clearCourseRelatedScores(int courseId);

    // 清理用户关联数据（删除用户前调用）
    bool clearUserRelatedData(int userId, UserRole role);
};

#endif
