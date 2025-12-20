#include "StudentModel.h"
#include <sstream>

StudentModel::StudentModel() : id(0) {}

StudentModel::StudentModel(const std::string &studentNo, const std::string &name, const std::string &gender,
                           const std::string &major, const std::string &grade, const std::string &phone)
    : id(0), studentNo(studentNo), name(name), gender(gender), major(major), grade(grade), phone(phone) {}

StudentModel::StudentModel(int id, const std::string &studentNo, const std::string &name, const std::string &gender,
                           const std::string &major, const std::string &grade, const std::string &phone)
    : id(id), studentNo(studentNo), name(name), gender(gender), major(major), grade(grade), phone(phone) {}

int StudentModel::getId() const
{
    return id;
}

std::string StudentModel::getStudentNo() const
{
    return studentNo;
}

std::string StudentModel::getName() const
{
    return name;
}

std::string StudentModel::getGender() const
{
    return gender;
}

std::string StudentModel::getMajor() const
{
    return major;
}

std::string StudentModel::getGrade() const
{
    return grade;
}

std::string StudentModel::getPhone() const
{
    return phone;
}

void StudentModel::setId(int id)
{
    this->id = id;
}

void StudentModel::setStudentNo(const std::string &studentNo)
{
    this->studentNo = studentNo;
}

void StudentModel::setName(const std::string &name)
{
    this->name = name;
}

void StudentModel::setGender(const std::string &gender)
{
    this->gender = gender;
}

void StudentModel::setMajor(const std::string &major)
{
    this->major = major;
}

void StudentModel::setGrade(const std::string &grade)
{
    this->grade = grade;
}

void StudentModel::setPhone(const std::string &phone)
{
    this->phone = phone;
}

std::string StudentModel::toString() const
{
    std::stringstream ss;
    ss << "StudentModel{id=" << id
       << ", studentNo=" << studentNo
       << ", name=" << name
       << ", gender=" << gender
       << ", major=" << major
       << ", grade=" << grade
       << ", phone=" << phone << "}";
    return ss.str();
}
