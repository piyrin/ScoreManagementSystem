#ifndef STUDENT_MODEL_H
#define STUDENT_MODEL_H

#include <string>

/**
 * 学生模型：存储学生基本信息
 * 关联关系：
 * - 与成绩模型（ScoreModel）通过studentId关联（一个学生有多条成绩记录）
 * - 与用户模型（UserModel）通过relatedId关联（用户表中的学生角色关联此id）
 */
class StudentModel
{
private:
    int id;                // 学生ID（数据库自增主键）
    std::string studentNo; // 学号（如：2023001，业务唯一标识）
    std::string name;      // 学生姓名
    std::string gender;    // 性别（如：男/女）
    std::string major;     // 专业（如：计算机科学与技术）
    std::string grade;     // 年级（如：2023级）
    std::string phone;     // 联系电话（可选，用于通知）

public:
    // 默认构造函数：初始化成员变量为默认值
    StudentModel();

    // 带参构造函数（创建新学生，无ID，ID由数据库生成）
    StudentModel(const std::string &studentNo, const std::string &name, const std::string &gender,
                 const std::string &major, const std::string &grade, const std::string &phone);

    // 带参构造函数（从数据库加载学生，包含完整ID）
    StudentModel(int id, const std::string &studentNo, const std::string &name, const std::string &gender,
                 const std::string &major, const std::string &grade, const std::string &phone);

    // Getter方法：获取私有成员变量
    int getId() const;
    std::string getStudentNo() const;
    std::string getName() const;
    std::string getGender() const;
    std::string getMajor() const;
    std::string getGrade() const;
    std::string getPhone() const;

    // Setter方法：修改私有成员变量
    void setId(int id);
    void setStudentNo(const std::string &studentNo);
    void setName(const std::string &name);
    void setGender(const std::string &gender);
    void setMajor(const std::string &major);
    void setGrade(const std::string &grade);
    void setPhone(const std::string &phone);

    // 调试辅助方法：转换为字符串输出
    std::string toString() const;
};

#endif // STUDENT_MODEL_H