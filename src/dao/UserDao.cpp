#include "UserDao.h"
#include <iostream>
#include <sqlite3.h>

// 构造函数：获取数据库连接
UserDao::UserDao()
{
    this->db = SqliteUtils::getInstance()->getDbConnection();
    if (this->db == nullptr)
    {
        std::cerr << "UserDao初始化失败:数据库连接获取失败" << std::endl;
    }
    else
    {
        std::cout << "UserDao初始化成功:数据库连接已建立" << std::endl;
    }
}

// 析构函数：释放资源
UserDao::~UserDao()
{
    std::cout << "UserDao资源释放" << std::endl;
}

// 1. 新增用户（注册）
bool UserDao::insert(const UserModel &user)
{
    if (this->db == nullptr)
        return false;

    // SQL：插入user表（id自增）
    std::string sql = "INSERT INTO user (username, password, role, relatedId) "
                      "VALUES (?, ?, ?, ?);";

    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "UserDao插入失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // 绑定参数（对应user表字段）
    sqlite3_bind_text(stmt, 1, user.getUsername().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, static_cast<int>(user.getRole())); // 角色转int
    sqlite3_bind_int(stmt, 4, user.getRelatedId());

    // 执行SQL（用户名唯一，重复会失败）
    ret = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (ret == SQLITE_DONE)
    {
        std::cout << "UserDao注册成功:" << user.getUsername() << std::endl;
        return true;
    }
    else
    {
        std::cerr << "UserDao注册失败:用户名已存在 - " << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }
}

// 2. 根据用户名查询用户（登录用）
UserModel UserDao::selectByUsername(const std::string &username)
{
    if (this->db == nullptr)
        return UserModel();

    // SQL：根据用户名查询
    std::string sql = "SELECT id, username, password, role, relatedId FROM user WHERE username = ?;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "UserDao查询失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return UserModel();
    }

    // 绑定用户名参数
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    // 解析结果
    UserModel user;
    ret = sqlite3_step(stmt);
    if (ret == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        std::string dbUsername = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::string dbPassword = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        UserRole role = static_cast<UserRole>(sqlite3_column_int(stmt, 3));
        int relatedId = sqlite3_column_int(stmt, 4);

        // 构造UserModel对象
        user = UserModel(id, dbUsername, dbPassword, role, relatedId);
    }
    else
    {
        std::cout << "UserDao查询失败:用户名不存在 - " << username << std::endl;
    }

    sqlite3_finalize(stmt);
    return user;
}

// 3. 根据ID查询用户
UserModel UserDao::selectById(int id)
{
    if (this->db == nullptr)
        return UserModel();

    std::string sql = "SELECT id, username, password, role, relatedId FROM user WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "UserDao查询失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return UserModel();
    }

    sqlite3_bind_int(stmt, 1, id);
    UserModel user;
    ret = sqlite3_step(stmt);
    if (ret == SQLITE_ROW)
    {
        std::string dbUsername = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::string dbPassword = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        UserRole role = static_cast<UserRole>(sqlite3_column_int(stmt, 3));
        int relatedId = sqlite3_column_int(stmt, 4);
        user = UserModel(id, dbUsername, dbPassword, role, relatedId);
    }

    sqlite3_finalize(stmt);
    return user;
}

// 4. 更新用户密码
bool UserDao::updatePassword(int id, const std::string &newPassword)
{
    if (this->db == nullptr)
        return false;

    std::string sql = "UPDATE user SET password = ? WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "UserDao更新密码失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_bind_text(stmt, 1, newPassword.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, id);

    ret = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (ret == SQLITE_DONE)
    {
        std::cout << "UserDao密码更新成功:用户ID=" << id << std::endl;
        return true;
    }
    else
    {
        std::cerr << "UserDao密码更新失败:用户ID不存在 - " << id << std::endl;
        return false;
    }
}

// 5. 根据ID删除用户
bool UserDao::deleteById(int id)
{
    if (this->db == nullptr)
        return false;

    std::string sql = "DELETE FROM user WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "UserDao删除失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);
    ret = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (ret == SQLITE_DONE)
    {
        std::cout << "UserDao删除成功:用户ID=" << id << std::endl;
        return true;
    }
    else
    {
        std::cerr << "UserDao删除失败:用户ID不存在 - " << id << std::endl;
        return false;
    }
}