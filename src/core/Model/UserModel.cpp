#include "UserModel.h"
#include <sstream>

// 默认构造函数：初始化成员变量为默认值
UserModel::UserModel() : id(0), role(0), relatedId(0) {}

// 带参构造：创建新用户
UserModel::UserModel(const std::string &username, const std::string &password, int role, int relatedId) : id(0), username(username), password(password), role(role), relatedId(relatedId) {}

// 带参构造：从数据库中加载用户
UserModel::UserModel(int id, const std::string &username, const std::string &password, int role, int relatedId) : id(id), username(username), password(password), role(role), relatedId(relatedId) {}

// Getter函数实现
int UserModel::getId() const
{
    return id;
}
std::string UserModel::getUsername() const
{
    return username;
}
std::string UserModel::getPassword() const
{
    return password;
}
int UserModel::getRole() const
{
    return role;
}
int UserModel::getRelatedId() const
{
    return relatedId;
}

// Setter函数实现
void UserModel::setId(int id)
{
    this->id = id;
}
void UserModel::setUsername(const std ::string &username)
{
    this->username = username;
}
void UserModel::setPassword(const std::string &password)
{
    this->password = password;
}
void UserModel::setRole(int role)
{
    this->role = role;
}
void UserModel::setRelatedId(int relatedId)
{
    this->relatedId = relatedId;
}

// 调试辅助函数：将对象转化为字符串
std::string UserModel::toString() const
{
    std::stringstream ss;
    ss << "UserModel{id=" << id
       << ",username='" << username
       << "',role=" << role
       << ",relatedId=" << relatedId
       << "}";
    return ss.str();
}