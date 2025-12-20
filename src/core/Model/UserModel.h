#ifndef USERMODEL_H
#define USERMODEL_H
#include <string>


// 角色：1—管理员，2-教师，3-学生
// relatedId：关联学生Id或教师Id

enum class UserRole
{
    ADMIN = 1,   // 管理员
    TEACHER = 2, // 教师
    STUDENT = 3  // 学生
};

class UserModel
{
private:
    int id;               // 用户ID，唯一标识数据库自增
    std::string username; // 用户名
    std::string password; // 登录密码
    UserRole role;        // 角色：1—管理员，2-教师，3-学生
    int relatedId;        // 关联ID，学生或老师
public:
    UserModel();
    // 创建用户，无id
    UserModel(const std::string &username, const std::string &password, UserRole role, int relatedId);
    //从数据库加载用户，有id
    UserModel(int id, const std::string &username, const std::string &password, UserRole role, int relatedId);

    // Getter函数
    int getId() const;
    std::string getUsername() const;
    std::string getPassword() const;
    UserRole getRole() const;
    int getRelatedId() const;

    // Setter函数
    void setId(int id);
    void setUsername(const std::string &username);
    void setPassword(const std::string &password);
    void setRole(UserRole role);
    void setRelatedId(int relatedId);

    // 调试辅助方法
    std::string toString() const;
    std::string getRoleStr() const;
};

#endif