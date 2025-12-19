#include "TeacherService.h"
#include "../dao/TeacherDao.h"
#include "../dao/CourseDao.h"
#include "../dao/ScoreDao.h"
#include "../dao/StudentDao.h"
#include <iostream>
#include <iomanip>

TeacherService::TeacherService()
{
    //初始化Dao层对象
    teacher_dao = new TeacherDao();
    course_dao = new CourseDao();
    student_dao = new StudentDao();
    score_dao = new ScoreDao();
}

TeacherService::~TeacherService()
{
    //释放Dao层资源
    delete teacher_dao;
    delete course_dao;
    delete student_dao;
    delete score_dao;
}

bool TeacherService::checkCoursePermission(const UserModel &login_user, int courseId)
{
    // 校验课程是否存在
    CourseModel course = course_dao->selectById(courseId);
    if (course.getId() == 0)
    {
        std::cerr << "权限校验失败:未找到ID为" << courseId << "的课程" << std::endl;
        return false;
    }

    // 校验课程的teacherId是否等于当前教师的ID（login_user.relatedId）
    int current_teacher_id = login_user.getRelatedId();
    if (course.getTeacherId() != current_teacher_id)
    {
        std::cerr << "权限校验失败：课程" << course.getCourseName() << "非本人授课" << std::endl;
        return false;
    }

    return true;
}

TeacherModel TeacherService::getTeacherInfo(const UserModel &login_user)
{
    //角色检验：验证是否是老师
    if(login_user.getRole()!=UserRole::TEACHER)
    {
        std::cerr << "教师信息查询失败：角色错误(当前角色：" << login_user.getRoleStr() << ")" << std::endl;
        return TeacherModel();
    }

    //从UserModel中获取关联的teacher.id
    int teacher_id = login_user.getRelatedId();
    if(teacher_id==0)
    {
        std::cerr << "教师信息查询失败:关联ID无效" << std::endl;
        return TeacherModel();
    }

    //调用Dao层查询教师信息
    TeacherModel teacher = teacher_dao->selectById(teacher_id);
    {
        if(teacher.getId()==0)
        {
            std::cerr << "教师信息查询失败:未找到ID为" << teacher_id << "的教师" << std::endl;
        }
        else
        {
            std::cout << "教师信息查询成功：" << teacher.toString() << std::endl;
        }
        return teacher;
    }
}

TeacherOpResult TeacherService::updateTeacherInfo(const UserModel &login_user, const TeacherModel &new_info)
{
    if(login_user.getRole()!=UserRole::TEACHER)
    {
        std::cerr << "修改个人信息失败：角色错误" << std::endl;
        return TeacherOpResult::ROLE_ERROR;
    }

    //校验参数
    if(new_info.getName().empty())
    {
        std::cerr << "修改个人信息失败：姓名不能为空" << std::endl;
        return TeacherOpResult::PARAM_ERROR;
    }

    // 获取当前教师ID
    int teacher_id = login_user.getRelatedId();
    if (teacher_id == 0)
    {
        std::cerr << "修改个人信息失败:关联ID无效" << std::endl;
        return TeacherOpResult::SYSTEM_ERROR;
    }

    // 查询原始信息（保留工号、性别，仅更新允许修改的字段）
    TeacherModel old_teacher = teacher_dao->selectById(teacher_id);
    if (old_teacher.getId() == 0)
    {
        std::cerr << "修改个人信息失败：未找到教师" << std::endl;
        return TeacherOpResult::NOT_FOUND;
    }

    // 构造更新对象：保留原始工号、性别，更新其他字段
    TeacherModel update_teacher = old_teacher;
    update_teacher.setName(new_info.getName());             // 姓名
    update_teacher.setTitle(new_info.getTitle());           // 职称
    update_teacher.setDepartment(new_info.getDepartment()); // 部门
    update_teacher.setEmail(new_info.getEmail());           // 邮箱

    // 执行更新
    if (teacher_dao->update(update_teacher))
    {
        std::cout << "修改个人信息成功：" << update_teacher.toString() << std::endl;
        return TeacherOpResult::SUCCESS;
    }
    else
    {
        std::cerr << "修改个人信息失败：数据库更新异常" << std::endl;
        return TeacherOpResult::SYSTEM_ERROR;
    }
}

std::vector<CourseModel> TeacherService::getMyCourse(const UserModel &login_user)
{
    vector<CourseModel> courses;
    // 校验角色
    if (login_user.getRole() != UserRole::TEACHER)
    {
        std::cerr << "课程查询失败：角色错误" << std::endl;
        return courses;
    }

    // 获取当前教师ID
    int teacher_id = login_user.getRelatedId();
    if (teacher_id == 0)
    {
        std::cerr << "课程查询失败:关联ID无效" << std::endl;
        return courses;
    }

    // 调用Dao层查询该教师授课的所有课程
    courses = course_dao->selectByTeacherId(teacher_id);
    if (courses.empty())
    {
        std::cout << "课程查询结果：暂无授课课程" << std::endl;
    }
    else
    {
        std::cout << "课程查询成功：共" << courses.size() << "门授课课程" << std::endl;
        // 打印课程详情
        for (const auto &course : courses)
        {
            std::cout << "课程ID:" << course.getId()
                      << " | 课程号：" << course.getCourseNo()
                      << " | 课程名：" << course.getCourseName()
                      << " | 学分：" << course.getCredit() << std::endl;
        }
    }

    return courses;
}

TeacherOpResult TeacherService::addStudentScore(const UserModel &login_user, const ScoreModel &score)
{
    // 校验角色
    if (login_user.getRole() != UserRole::TEACHER)
    {
        std::cerr << "成绩录入失败：角色错误" << std::endl;
        return TeacherOpResult::ROLE_ERROR;
    }

    // 校验参数：studentId、courseId、score、examTime不能为空/无效
    if (score.getStudentId() == 0 || score.getCourseId() == 0 || score.getScore() < 0 || score.getExamTime().empty())
    {
        std::cerr << "成绩录入失败:参数无效(学生ID/课程ID/分数/考试时间不能为空)" << std::endl;
        return TeacherOpResult::PARAM_ERROR;
    }

    // 校验课程权限（是否为本人授课）
    if (!checkCoursePermission(login_user, score.getCourseId()))
    {
        return TeacherOpResult::NO_PERMISSION;
    }

    // 校验学生是否存在
    if (student_dao->selectById(score.getStudentId()).getId() == 0)
    {
        std::cerr << "成绩录入失败:未找到ID为" << score.getStudentId() << "的学生" << std::endl;
        return TeacherOpResult::NOT_FOUND;
    }

    // 校验成绩是否重复（学生+课程唯一）
    std::vector<ScoreModel> exist_scores = score_dao->selectByStudentIdAndCourseId(score.getStudentId(), score.getCourseId());
    if (!exist_scores.empty())
    {
        std::cerr << "成绩录入失败:学生ID=" << score.getStudentId() << " 课程ID=" << score.getCourseId() << " 已存在成绩" << std::endl;
        return TeacherOpResult::DUPLICATE_SCORE;
    }

    // 执行成绩录入（确保成绩的teacherId与当前教师一致）
    ScoreModel add_score = score;
    add_score.setTeacherId(login_user.getRelatedId()); // 强制绑定当前教师ID
    if (score_dao->insert(add_score))
    {
        std::cout << "成绩录入成功:学生ID=" << add_score.getStudentId()
                  << " 课程ID=" << add_score.getCourseId()
                  << " 分数=" << add_score.getScore() << std::endl;
        return TeacherOpResult::SUCCESS;
    }
    else
    {
        std::cerr << "成绩录入失败：数据库插入异常" << std::endl;
        return TeacherOpResult::SYSTEM_ERROR;
    }
}
se
std::vector<std::pair<StudentModel, ScoreModel>> TeacherService ::getCourseScores(const UserModel &login_user, int couresId)
{
    std::vector<std::pair<StudentModel, ScoreModel>> result;
    // 校验角色
    if (login_user.getRole() != UserRole::TEACHER)
    {
        std::cerr << "课程成绩查询失败：角色错误" << std::endl;
        return result;
    }

    // 校验课程权限
    if (!checkCoursePermission(login_user, courseId))
    {
        return result;
    }

    //查询该课所有成绩
    std::vector<ScoreModel> scores = score_dao->selectByCourseId(couresId);
    if(scores.empty())
    {
        std::cout << "课程成绩查询结果：暂无学生成绩" << std::endl;
        return result;
    }

    //关联学生信息 成绩->学生id->学生信息
    for(const auto&score:scores )
    {Student
        StudentModel student = student_dao->selectById(score.getCourseId());
        if(student.getId()!=0)
        {
            result.emplace_back(student, score);
        }
        else
        {
            std::cerr << "成绩ID=" << score.getId() << " 关联的学生不存在" << std::endl;
        }
    }
    // 打印结果
    std::cout << "课程成绩查询成功：共" << result.size() << "条成绩记录" << std::endl;
    for (const auto &pair : result)
    {
        const StudentModel &student = pair.first;
        const ScoreModel &score = pair.second;
        std::cout << "学生学号：" << student.getStudentNo()
                  << " | 学生姓名：" << student.getName()
                  << " | 分数：" << score.getScore()
                  << " | 考试时间：" << score.getExamTime() << std::endl;
    }

    return result;
}

std::pair<double, double> TeacherService::statCourseScore(const std::vector<std::pair<StudentModel, ScoreModel>> &courseScores)
{
    double avg_score = 0.0;
    double pass_rate = 0.0;
    int total = courseScores.size();

    if (total == 0)
    {
        std::cout << "成绩统计：暂无成绩数据，无法统计" << std::endl;
        return {avg_score, pass_rate};
    }

    // 计算总分和及格人数（60分及格）
    double total_score = 0.0;
    int pass_count = 0;
    for (const auto &pair : courseScores)
    {
        double score = pair.second.getScore();
        total_score += score;
        if (score >= 60)
        {
            pass_count++;
        }
    }

    // 计算平均分（保留2位小数）
    avg_score = std::round((total_score / total) * 100) / 100;
    // 计算及格率（保留2位小数，百分比）
    pass_rate = std::round(((double)pass_count / total) * 10000) / 100;

    // 打印统计结果
    std::cout << "\n===== 课程成绩统计 =====" << std::endl;
    std::cout << "总人数：" << total << " 人" << std::endl;
    std::cout << "平均分：" << avg_score << " 分" << std::endl;
    std::cout << "及格人数：" << pass_count << " 人" << std::endl;
    std::cout << "及格率：" << pass_rate << "%" << std::endl;

    return {avg_score, pass_rate};
}