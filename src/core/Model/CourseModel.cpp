#include "CourseModel.h"
#include <sstream>

CourseModel::CourseModel() : id(0), credit(0), teacherId(0) {}

CourseModel::CourseModel(int id, const std::string &courseNo, const std::string &courseName, int credit, int teacherId, const std::string &description) : id(id), courseNo(courseNo), courseName(courseName), credit(credit), teacherId(teacherId), description(description) {}

int CourseModel::getId() const
{
    return id;
}
std::string CourseModel::getCourseNo() const
{
    return courseNo;
}
std::string CourseModel::getCourseName() const
{
    return courseName;
}
int CourseModel::getCredit() const
{
    return credit;
}
int CourseModel::getTeacherId() const
{
    return teacherId;
}
std::string CourseModel::getDescription() const
{
    return description;
}

void CourseModel::setId(int id)
{
    this->id = id;
}
void CourseModel::setCourseNo(const std::string &courseNo)
{
    this->courseNo = courseNo;
}
void CourseModel::setCourseName(const std::string &courseName)
{
    this->courseName = courseName;
}
void CourseModel::setCredit(int credit)
{
    this->credit = credit;
}
void CourseModel::setTeacherId(int teacherId)
{
    this->teacherId = teacherId;
}
void CourseModel::setDescription(const std::string &description)
{
    this->description = description;
}

std::string CourseModel::toString() const
{
    std::stringstream ss;
    ss << "CourseModel{id=" << id
       << ", courseNo='" << courseNo
       << "', courseName='" << courseName
       << "', credit=" << credit
       << ", teacherId=" << teacherId
       << ", description='" << description
       << "'}";
    return ss.str();
}