#ifndef USER_DAO_H
#define USER_DAO_H

#include <string>
#include "../core/Model/UserModel.h" // 依赖用户模型
#include "../utils/SqliteUtils.h"    // 依赖SQLite工具类

/**
 * 用户数据访问对象（必须文件）
 * 功能：操作user表（存储登录信息）
 * 核心字段：username（登录名）、password（加密密码）、role（角色）、relatedId（关联学生/教师ID）
 */
class UserDao
{
public:
    // 构造函数：初始化数据库连接
    UserDao();

    // 析构函数：释放资源
    ~UserDao();

    // 1. 新增用户（注册）
    // 参数：UserModel对象（无id，id自增）
    // 返回：true-成功，false-失败（如用户名重复）
    bool insert(const UserModel &user);

    // 2. 根据用户名查询用户（登录校验）
    // 参数：登录用户名（username）
    // 返回：查询到的UserModel（id=0表示未找到）
    UserModel selectByUsername(const std::string &username);

    // 3. 根据ID查询用户
    // 参数：user表的id
    // 返回：查询到的UserModel
    UserModel selectById(int id);

    // 4. 更新用户密码
    // 参数：user id + 新密码（已加密）
    // 返回：true-成功，false-失败
    bool updatePassword(int id, const std::string &newPassword);

    // 5. 根据ID删除用户
    // 参数：user表的id
    // 返回：true-成功，false-失败
    bool deleteById(int id);

private:
    // SQLite数据库连接句柄
    sqlite3 *db;
};

#endif // USER_DAO_H