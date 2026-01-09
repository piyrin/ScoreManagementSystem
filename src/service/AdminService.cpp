#include "AdminService.h"
#include "../dao/UserDao.h"
#include "../dao/StudentDao.h"
#include "../dao/TeacherDao.h"
#include "../dao/CourseDao.h"
#include "../dao/ScoreDao.h"
#include <iostream>
#include <iomanip>
#include <cmath>

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
    if (login_admin.getRole() != UserRole::ADMIN)
    {
        std::cerr << "角色错误:当前角色非管理员(角色:" << login_admin.getRoleStr() << ")" << std::endl;
        return false;
    }
    return true;
}

bool AdminService::clearCourseRelatedScores(int courseId)
{
    std::vector<ScoreModel> scores = score_dao->selectByCourseId(courseId);
    if (scores.empty())
    {
        return true;
    }

    bool allDelete = true;
    for (const auto score : scores)
    {
        if (!score_dao->deleteById(score.getId()))
        {
            std::cerr << "清除成绩失败:成绩Id:" << score.getId() << std::endl;
            return false;
        }
    }
    return allDelete;
}

bool AdminService::clearUserRelatedData(int userId, UserRole role)
{
    UserModel user = user_dao->selectById(userId);
    if (user.getId() == 0)
    {
        return false;
    }

    int relatedId = user.getRelatedId();
    bool success = true;

    switch (role)
    {
    case UserRole::STUDENT:
    {
        std::vector<ScoreModel> student_scores = score_dao->selectByStudentId(relatedId);
        for (const auto &score : student_scores)
        {
            if (!score_dao->deleteById(score.getId()))
            {
                std::cerr << "清理学生成绩失败:成绩ID=" << score.getId() << std::endl;
                success = false;
            }
        }
        // 删除学生表中的记录，释放学号
        if (!student_dao->deleteById(relatedId))
        {
            std::cerr << "清理学生信息失败:学生ID=" << relatedId << std::endl;
            success = false;
        }
        break;
    }

    case UserRole::TEACHER:
    {
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
    }
    case UserRole::ADMIN:
        break;
    default:
        success = false;
        break;
    }
    return success;
}

AdminOpResult AdminService::addStudentUser(const UserModel &login_admin, const std::string &username, const std::string &password, const StudentModel &studentInfo)
{
    if (!checkAdminRole(login_admin))
        return AdminOpResult::ROLE_ERROR;
    if (username.empty() || password.empty() || studentInfo.getStudentNo().empty() || studentInfo.getName().empty())
    {
        std::cerr << "新增学生用户失败:必要信息填写不完整" << std::endl;
        return AdminOpResult::PARAM_ERROR;
    }

    if (user_dao->selectByUsername(username).getId() != 0)
    {
        std::cerr << "新增用户失败,用户名:" << username << "已存在" << std::endl;
    }

    if (student_dao->selectByStudentNo(studentInfo.getStudentNo()).getId() != 0)
    {
        std::cerr << "新增学生用户失败：学号" << studentInfo.getStudentNo() << "已存在" << std::endl;
        return AdminOpResult::DUPLICATE_DATA;
    }

    if (!student_dao->insert(studentInfo))
    {
        std::cerr << "新增学生用户失败：插入学生信息异常" << std::endl;
        return AdminOpResult::SYSTEM_ERROR;
    }

    StudentModel new_student = student_dao->selectByStudentNo(studentInfo.getStudentNo());
    if (new_student.getId() == 0)
    {
        std::cerr << "新增学生用户失败:获取学生ID异常" << std::endl;
        student_dao->deleteById(new_student.getId()); // 回滚
        return AdminOpResult::SYSTEM_ERROR;
    }

    UserModel new_user(0, username, password, UserRole::STUDENT, new_student.getId());
    if (user_dao->insert(new_user))
    {
        std::cout << "新增学生用户成功：" << new_user.toString() << std::endl;
        return AdminOpResult::SUCCESS;
    }
    else
    {
        student_dao->deleteById(new_student.getId());
        std::cerr << "新增学生用户失败" << std::endl;
        return AdminOpResult::SYSTEM_ERROR;
    }
}

AdminOpResult AdminService::addTeacherUser(const UserModel &login_admin, const std::string &username, const std::string &password, const TeacherModel &teacherInfo)
{
    if (!checkAdminRole(login_admin))
        return AdminOpResult::ROLE_ERROR;

    if (username.empty() || password.empty() || teacherInfo.getTeacherNo().empty() || teacherInfo.getName().empty())
    {
        std::cerr << "新增教师用户失败：必填参数为空" << std::endl;
        return AdminOpResult::PARAM_ERROR;
    }

    if (user_dao->selectByUsername(username).getId() != 0)
    {
        std::cerr << "新增教师用户失败：用户名" << username << "已存在" << std::endl;
        return AdminOpResult::DUPLICATE_DATA;
    }

    if (teacher_dao->selectByTeacherNo(teacherInfo.getTeacherNo()).getId() != 0)
    {
        std::cerr << "新增教师用户失败:工号" << teacherInfo.getTeacherNo() << "已存在" << std::endl;
        return AdminOpResult::DUPLICATE_DATA;
    }

    if (!teacher_dao->insert(teacherInfo))
    {
        std::cerr << "新增教师用户失败:插入教师信息异常" << std::endl;
        return AdminOpResult::SYSTEM_ERROR;
    }

    TeacherModel new_teacher = teacher_dao->selectByTeacherNo(teacherInfo.getTeacherNo());
    if (new_teacher.getId() == 0)
    {
        std::cerr << "新增教师用户失败:获取教师ID异常" << std::endl;
        teacher_dao->deleteById(new_teacher.getId()); // 回滚
        return AdminOpResult::SYSTEM_ERROR;
    }

    UserModel new_user(0, username, password, UserRole::TEACHER, new_teacher.getId());
    if (user_dao->insert(new_user))
    {
        std::cout << "新增教师用户成功：" << new_user.toString() << std::endl;
        return AdminOpResult::SUCCESS;
    }
    else
    {
        teacher_dao->deleteById(new_teacher.getId());
        std::cerr << "新增教师用户失败" << std::endl;
        return AdminOpResult::SYSTEM_ERROR;
    }
}

AdminOpResult AdminService::deleteUser(const UserModel &login_admin, int userId)
{
    if (!checkAdminRole(login_admin))
        return AdminOpResult::ROLE_ERROR;

    if (userId == 0)
    {
        std::cerr << "删除用户失败:用户ID无效" << std::endl;
        return AdminOpResult::PARAM_ERROR;
    }

    // 查询用户是否存在
    UserModel user = user_dao->selectById(userId);
    if (user.getId() == 0)
    {
        std::cerr << "删除用户失败:未找到ID为" << userId << "的用户" << std::endl;
        return AdminOpResult::NOT_FOUND;
    }

    if (!clearUserRelatedData(userId, user.getRole()))
    {
        std::cerr << "删除用户失败：关联数据清理异常" << std::endl;
        return AdminOpResult::SYSTEM_ERROR;
    }

    if (user_dao->deleteById(userId))
    {
        std::cout << "删除用户成功:用户ID=" << userId << "," << user.toString() << std::endl;
        return AdminOpResult::SUCCESS;
    }
    else
    {
        std::cerr << "删除用户失败" << std::endl;
        return AdminOpResult::SYSTEM_ERROR;
    }
}

AdminOpResult AdminService::updateStudent(const UserModel &login_admin, const StudentModel &student)
{
    if (!checkAdminRole(login_admin))
        return AdminOpResult::ROLE_ERROR;

    if (student.getId() == 0 || student.getName().empty())
    {
        std::cerr << "修改学生信息失败：参数无效" << std::endl;
        return AdminOpResult::PARAM_ERROR;
    }

    StudentModel oldStudent = student_dao->selectById(student.getId());
    if (oldStudent.getId() == 0)
    {
        std::cerr << "修改学生信息失败：未找到学生" << std::endl;
        return AdminOpResult::NOT_FOUND;
    }

    // 如果修改了学号，需要检查唯一性
    if (student.getStudentNo() != oldStudent.getStudentNo())
    {
        StudentModel existStudent = student_dao->selectByStudentNo(student.getStudentNo());
        if (existStudent.getId() != 0 && existStudent.getId() != student.getId())
        {
            std::cerr << "修改学生信息失败：学号" << student.getStudentNo() << "已存在" << std::endl;
            return AdminOpResult::DUPLICATE_DATA;
        }
    }

    if (student_dao->update(student))
    {
        std::cout << "修改学生信息成功：" << student.toString() << std::endl;
        return AdminOpResult::SUCCESS;
    }
    else
    {
        std::cerr << "修改学生信息失败" << std::endl;
        return AdminOpResult::SYSTEM_ERROR;
    }
}

AdminOpResult AdminService::updateTeacher(const UserModel &login_admin, const TeacherModel &teacher)
{
    if (!checkAdminRole(login_admin))
        return AdminOpResult::ROLE_ERROR;

    if (teacher.getId() == 0 || teacher.getName().empty())
    {
        std::cerr << "修改教师信息失败：参数无效" << std::endl;
        return AdminOpResult::PARAM_ERROR;
    }

    TeacherModel oldTeacher = teacher_dao->selectById(teacher.getId());
    if (oldTeacher.getId() == 0)
    {
        std::cerr << "修改教师信息失败：未找到教师" << std::endl;
        return AdminOpResult::NOT_FOUND;
    }

    // 如果修改了工号，需要检查唯一性
    if (teacher.getTeacherNo() != oldTeacher.getTeacherNo())
    {
        TeacherModel existTeacher = teacher_dao->selectByTeacherNo(teacher.getTeacherNo());
        if (existTeacher.getId() != 0 && existTeacher.getId() != teacher.getId())
        {
            std::cerr << "修改教师信息失败：工号" << teacher.getTeacherNo() << "已存在" << std::endl;
            return AdminOpResult::DUPLICATE_DATA;
        }
    }

    if (teacher_dao->update(teacher))
    {
        std::cout << "修改教师信息成功：" << teacher.toString() << std::endl;
        return AdminOpResult::SUCCESS;
    }
    else
    {
        std::cerr << "修改教师信息失败" << std::endl;
        return AdminOpResult::SYSTEM_ERROR;
    }
}

AdminOpResult AdminService::addCourse(const UserModel &login_admin, const CourseModel &course)
{
    if (!checkAdminRole(login_admin))
        return AdminOpResult::ROLE_ERROR;

    if (course.getCourseNo().empty() || course.getCourseName().empty() || course.getCredit() <= 0 || course.getTeacherId() == 0)
    {
        std::cerr << "新增课程失败：必填参数无效" << std::endl;
        return AdminOpResult::PARAM_ERROR;
    }
    if (course_dao->selectByCourseNo(course.getCourseNo()).getId() != 0)
    {
        std::cerr << "新增课程失败：课程号" << course.getCourseNo() << "已存在" << std::endl;
        return AdminOpResult::DUPLICATE_DATA;
    }

    if (teacher_dao->selectById(course.getTeacherId()).getId() == 0)
    {
        std::cerr << "新增课程失败:未找到ID为" << course.getTeacherId() << "的教师" << std::endl;
        return AdminOpResult::NOT_FOUND;
    }

    // 插入课程
    if (course_dao->insert(course))
    {
        std::cout << "新增课程成功：" << course.toString() << std::endl;
        return AdminOpResult::SUCCESS;
    }
    else
    {
        std::cerr << "新增课程失败：数据库插入异常" << std::endl;
        return AdminOpResult::SYSTEM_ERROR;
    }
}

AdminOpResult AdminService::updateCourse(const UserModel &login_admin, const CourseModel &course)
{
    if (!checkAdminRole(login_admin))
        return AdminOpResult::ROLE_ERROR;

    if (course.getId() == 0 || course.getCourseNo().empty() || course.getCourseName().empty() || course.getCredit() <= 0 || course.getTeacherId() == 0)
    {
        std::cerr << "修改课程失败：必填参数无效" << std::endl;
        return AdminOpResult::PARAM_ERROR;
    }

    if (course_dao->selectById(course.getId()).getId() == 0)
    {
        std::cerr << "修改课程失败:未找到ID为" << course.getId() << "的课程" << std::endl;
        return AdminOpResult::NOT_FOUND;
    }

    if (teacher_dao->selectById(course.getTeacherId()).getId() == 0)
    {
        std::cerr << "修改课程失败:未找到ID为" << course.getTeacherId() << "的教师" << std::endl;
        return AdminOpResult::NOT_FOUND;
    }

    CourseModel existCourse = course_dao->selectByCourseNo(course.getCourseNo());
    if (existCourse.getId() != 0 && existCourse.getId() != course.getId())
    {
        std::cerr << "修改课程失败：课程号" << course.getCourseNo() << "已被其他课程占用" << std::endl;
        return AdminOpResult::DUPLICATE_DATA;
    }

    if (course_dao->update(course))
    {
        std::cout << "修改课程成功：" << course.toString() << std::endl;
        return AdminOpResult::SUCCESS;
    }
    else
    {
        std::cerr << "修改课程失败" << std::endl;
        return AdminOpResult::SYSTEM_ERROR;
    }
}

AdminOpResult AdminService::deleteCourse(const UserModel &login_admin, int courseId)
{
    if (!checkAdminRole(login_admin))
        return AdminOpResult::ROLE_ERROR;

    if (courseId == 0)
    {
        std::cerr << "删除课程失败:课程ID无效" << std::endl;
        return AdminOpResult::PARAM_ERROR;
    }

    if (course_dao->selectById(courseId).getId() == 0)
    {
        std::cerr << "删除课程失败:未找到ID为" << courseId << "的课程" << std::endl;
        return AdminOpResult::NOT_FOUND;
    }

    if (!clearCourseRelatedScores(courseId))
    {
        std::cerr << "删除课程失败：关联成绩清理异常" << std::endl;
        return AdminOpResult::SYSTEM_ERROR;
    }

    if (course_dao->deleteById(courseId))
    {
        std::cout << "删除课程成功:课程ID=" << courseId << std::endl;
        return AdminOpResult::SUCCESS;
    }
    else
    {
        std::cerr << "删除课程失败：数据库删除异常" << std::endl;
        return AdminOpResult::SYSTEM_ERROR;
    }
}

std::vector<CourseModel> AdminService::queryAllCourses(const UserModel &login_admin)
{
    std::vector<CourseModel> courses;

    if (!checkAdminRole(login_admin))
        return courses;

    courses = course_dao->selectAll();

    std::cout << "查询课程成功：共" << courses.size() << "门课程" << std::endl;
    for (const auto &course : courses)
    {
        std::cout << course.toString() << std::endl;
    }

    return courses;
}
