#ifndef COURSE_MODEL_H
#define COURSE_MODEL_H
#include <string>

class CourseModel
{
private:
    int id;                  // 课程ID
    std::string courseNo;    // 课程号（如：CS101）
    std::string courseName;  // 课程名称
    int credit;              // 学分
    int teacherId;           // 授课教师ID（关联teacher表）
    std::string description; // 课程描述

public:
    CourseModel();

    // 带参构造函数（从数据库加载课程，包含完整ID）
    CourseModel(int id, const std::string &courseNo, const std::string &courseName, int credit, int teacherId, const std::string &description);

    // 获取私有成员变量的值
    int getId() const;
    std::string getCourseNo() const;
    std::string getCourseName() const;
    int getCredit() const;
    int getTeacherId() const;
    std::string getDescription() const;

    // 修改私有成员变量的值
    void setId(int id);
    void setCourseNo(const std::string &courseNo);
    void setCourseName(const std::string &courseName);
    void setCredit(int credit);
    void setTeacherId(int teacherId);
    void setDescription(const std::string &description);

    //将课程对象转换为字符串
    std::string toString() const;
};
#endif