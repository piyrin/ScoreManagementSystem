#include"ScoreModel.h"
#include<sstream>

//默认构造
ScoreModel::ScoreModel():id(0),studentId(0),courseId(0),score(0.0),teacherId(0){}

//有参构造，创建成绩
ScoreModel::ScoreModel(int studentId, int courseId, double score, std::string examTime, int teacherId):studentId(studentId),courseId(courseId),score(score),examTime(examTime),teacherId(teacherId){}

//有参构造，从数据库加载成绩
ScoreModel::ScoreModel(int id, int studentId, int courseId, double score, std::string examTime, int teacherId):id(id),studentId(studentId),courseId(courseId),score(score),examTime(examTime),teacherId(teacherId){}

//Getter函数实现
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
    if(score>=90&&score<=100)
        return 4.0;
    else if(score>=85&&score<=89)
        return 3.7;
    else if(score<=84&&score>=82)
        return 3.3;
    else if(score>=78&&score<=81)
        return 3.0;
    else if(score>=75&&score<=77)
        return 2.7;
    else if(score>=72&&score<=74)
        return 2.3;
    else if(score>=68&&score<=71)
        return 2.0;
    else if(score>=64&&score<=67)
        return 1.5;
    else if(score>=60&&score<=63)
        return 1.0;
    else
        return 0.0;
}

//Setter函数实现
void ScoreModel::setId(int id)
{
    this->id = id;
}
void ScoreModel::setStudentId(int studentId)
{
    this->studentId=studentId;
}
void ScoreModel::setCourseId(int courseId)
{
    this->courseId = courseId;
}
void ScoreModel::setScore(double score)
{
    this->score = score;
}
void ScoreModel::setExamTime(const std::string&examTime)
{
    this->examTime=examTime;
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