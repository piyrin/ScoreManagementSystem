#ifndef COURSE_MODEL_H
#define COURSE_MODEL_H
#include<string>

class CourseModel
{
private:
    int id;                  // 课程ID（唯一标识，数据库自增）
    std::string courseNo;    // 课程号（如：CS101）
    std::string courseName;  // 课程名称（如：C++程序设计）
    int credit;              // 学分（关键字段：用于加权平均绩点计算）
    int teacherId;           // 授课教师ID（外键：关联teacher表）
    std::string description; // 课程描述（如课程简介）

public:
    // 默认构造函数：初始化所有成员变量为默认值
    CourseModel();

    // 带参构造函数（创建新课程，无ID，ID由数据库生成）
    CourseModel(const std::string &courseNo, const std::string &courseName, int credit, int teacherId, const std::string &description);

    // 带参构造函数（从数据库加载课程，包含完整ID）
    CourseModel(int id, const std::string &courseNo, const std::string &courseName, int credit, int teacherId, const std::string &description);
    
    // Getter方法：获取私有成员变量的值
    int getId() const;
    std::string getCourseNo() const;
    std::string getCourseName() const;
    int getCredit() const; // 学分：加权平均绩点计算的核心
    int getTeacherId() const;
    std::string getDescription() const;

    // Setter方法：修改私有成员变量的值
    void setId(int id);
    void setCourseNo(const std::string &courseNo);
    void setCourseName(const std::string &courseName);
    void setCredit(int credit);
    void setTeacherId(int teacherId);
    void setDescription(const std::string &description);

    // 调试辅助方法：将课程对象转换为字符串，便于输出测试
    std::string toString() const;
};
#endif