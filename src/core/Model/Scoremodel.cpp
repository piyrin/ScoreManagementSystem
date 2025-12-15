#include "ScoreModel.h"
#include <sstream>

// 默认构造
ScoreModel::ScoreModel() : id(0), studentId(0), courseId(0), score(0.0), teacherId(0) {}

// 有参构造，创建成绩
ScoreModel::ScoreModel(int studentId, int courseId, double score, std::string examTime, int teacherId) : id(0), studentId(studentId), courseId(courseId), score(score), examTime(examTime), teacherId(teacherId) {}

// 有参构造，从数据库加载成绩
ScoreModel::ScoreModel(int id, int studentId, int courseId, double score, std::string examTime, int teacherId) : id(id), studentId(studentId), courseId(courseId), score(score), examTime(examTime), teacherId(teacherId) {}

// Getter函数实现
int ScoreModel::getId() const
{
    return id;
}
int ScoreModel::getStudentId() const
{
    return studentId;
}
int ScoreModel::getCourseId() const
{
    return courseId;
}
double ScoreModel::getScore() const
{
    return score;
}
std::string ScoreModel::getExamTime() const
{
    return examTime;
}
int ScoreModel::getTeacherId() const
{
    return teacherId;
}

double ScoreModel::getGpa() const
{
    if (score >= 90)
        return 4.0;
    if (score >= 85)
        return 3.7;
    if (score >= 82)
        return 3.3;
    if (score >= 78)
        return 3.0;
    if (score >= 75)
        return 2.7;
    if (score >= 72)
        return 2.3;
    if (score >= 68)
        return 2.0;
    if (score >= 64)
        return 1.5;
    if (score >= 60)
        return 1.0;
    return 0.0;
}

// Setter函数实现
void ScoreModel::setId(int id)
{
    this->id = id;
}
void ScoreModel::setStudentId(int studentId)
{
    this->studentId = studentId;
}
void ScoreModel::setCourseId(int courseId)
{
    this->courseId = courseId;
}
void ScoreModel::setScore(double score)
{
    this->score = score;
}
void ScoreModel::setExamTime(const std::string &examTime)
{
    this->examTime = examTime;
}
void ScoreModel::setTeacherId(int teacherId)
{
    this->teacherId = teacherId;
}

std::string ScoreModel::toString() const
{
    std::stringstream ss;
    ss << "ScoreModel{id=" << id
       << ", studentId=" << studentId
       << ", courseId=" << courseId
       << ", score=" << score
       << ", gpa=" << getGpa() // 输出时自动计算并显示绩点
       << ", examTime='" << examTime
       << "', teacherId=" << teacherId
       << "}";
    return ss.str();
}