#ifndef TEACHER_MODEL_H
#define TEACHER_MODEL_H
#include <string>
// 教师模型，定义教师结构
// 教师与CourseModel通过teacherId关联
// 一个老师可教多门课程

class TeacherModel
{
private:
    int id;                 //教师id
    std::string teacherNo;  // 教师工号
    std::string name;       // 教师姓名
    std::string gender;     // 教师性别
    std::string title;      // 教师职称
    std::string department; // 教师部门
    std::string email;      // 教师邮箱

public:

    TeacherModel();
    // 带参构造，无Id，创建教师
    TeacherModel(const std::string &teacherNo,const std::string &name,const std::string &gender,const std::string &title,const std::string &department,const std::string &email);
    // 带参构造，有Id，从数据库加载id
    TeacherModel(int id, const std::string &teacherNo, const std::string &name, const std::string &gender, const std::string &title, const std::string &department, const std::string &email);

    // Getter函数
    int getId() const;
    std::string getTeacherNo() const;
    std::string getName() const;
    std::string getGender() const;
    std::string getTitle() const;
    std::string getDepartment() const;
    std::string getEmail() const;

    // Setter函数
    void setId(int id);
    void setTeacherNo(const std::string &teacherNo);
    void setName(const std::string &name);
    void setGender(const std::string &gender);
    void setTitle(const std::string &title);
    void setDepartment(const std::string &department);
    void setEmail(const std::string &email);

    // 调试辅助方法
    std::string toString() const;
};
#endif