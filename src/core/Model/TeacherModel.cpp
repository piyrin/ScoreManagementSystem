#include "TeacherModel.h"
#include <sstream>

// 默认构造
TeacherModel::TeacherModel() : id(0) {}

// 带参构造，无Id
TeacherModel::TeacherModel(const std::string &teacherNo,const std::string &name,const std::string &gender,const std::string &title,const std::string &department,const std::string &email)
    : id(0), teacherNo(teacherNo), name(name), gender(gender), title(title), department(department), email(email) {}

// 带参构造，有Id
TeacherModel::TeacherModel(int id, const std::string &teacherNo, const std::string &name, const std::string &gender, const std::string &title, const std::string &department, const std::string &email): id(id), teacherNo(teacherNo), name(name), gender(gender), title(title), department(department), email(email) {}

// Getter函数
int TeacherModel::getId() const 
{ 
    return id; 
}
std::string TeacherModel::getTeacherNo() const 
{ 
    return teacherNo; 
}
std::string TeacherModel::getName() const 
{ 
    return name; 
}
std::string TeacherModel::getGender() const 
{ 
    return gender; 
}
std::string TeacherModel::getTitle() const 
{ 
    return title; 
}
std::string TeacherModel::getDepartment() const 
{ 
    return department; 
}
std::string TeacherModel::getEmail() const 
{ 
    return email; 
}

// Setter函数
void TeacherModel::setId(int id) 
{ 
    this->id = id; 
}
void TeacherModel::setTeacherNo(const std::string &teacherNo) 
{ 
    this->teacherNo = teacherNo; 
}
void TeacherModel::setName(const std::string &name) 
{ 
    this->name = name; 
}
void TeacherModel::setGender(const std::string &gender) 
{ 
    this->gender = gender; 
}
void TeacherModel::setTitle(const std::string &title) 
{ 
    this->title = title; 
}
void TeacherModel::setDepartment(const std::string &department) 
{ 
    this->department = department; 
}
void TeacherModel::setEmail(const std::string &email) 
{ 
    this->email = email; 
}

// 调试辅助方法
std::string TeacherModel::toString() const
{
    std::stringstream ss;
    ss << "TeacherModel{id=" << id
       << ", teacherNo=" << teacherNo
       << ", name=" << name
       << ", gender=" << gender
       << ", title=" << title
       << ", department=" << department
       << ", email=" << email << "}";
    return ss.str();
}
