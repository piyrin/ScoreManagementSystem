#ifndef USER_DAO_H
#define USER_DAO_H

#include <string>
#include <vector>
#include "../core/Model/UserModel.h"
#include "../utils/SqliteUtils.h"

class UserDao
{
public:
    // 初始化数据库连接
    UserDao();

    ~UserDao();

    // 新增用户（注册）
    bool insert(const UserModel &user);

    // 根据用户名查询用户
    UserModel selectByUsername(const std::string &username);

    // 根据ID查询用户
    UserModel selectById(int id);

    // 更新用户密码
    bool updatePassword(int id, const std::string &newPassword);

    // 更新用户信息
    bool update(const UserModel &user);

    // 根据ID删除用户
    bool deleteById(int id);

    // 查询所有用户
    std::vector<UserModel> selectAll();

private:
    // SQLite数据库连接句柄
    sqlite3 *db;
};

#endif