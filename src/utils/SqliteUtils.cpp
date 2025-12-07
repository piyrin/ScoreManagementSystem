#include <sqlite3.h>
#include <iostream>
#include "SqliteUtils.h"
#include <string>
#include "../config/Config.h"

// 静态成员变量初始化：单例指针
SqliteUtils *SqliteUtils::instance = nullptr;

SqliteUtils *SqliteUtils::getInstance()
{
    if (instance == nullptr)
    {
        instance = new SqliteUtils();
    }
    return instance;
}

SqliteUtils::SqliteUtils()
{
    // 从config模块获取数据库路径，关联之前模块
    Config *config = Config::getInstance();
    std::string dbPath = config->getPathConfig().db_path;

    // 打开SQLite数据库，若不存在会自动创建
    int ret = sqlite3_open(dbPath.c_str(), &conn);
    // 连接失败，打印错误信息
    if (ret != SQLITE_OK)
    {
        std::cerr << "数据库连接失败：" << sqlite3_errmsg(conn) << std::endl;
        conn = nullptr;
    }
    else
    {
        std::cout << "数据库连接成功：" << dbPath << std::endl;
    }
}

SqliteUtils::~SqliteUtils()
{
    if (conn != nullptr)
    {
        sqlite3_close(conn); // 关闭数据库
        conn = nullptr;
        std::cout << "数据库连接已关闭" << std::endl;
    }
}

bool SqliteUtils::executeUpdate(const std::string &sql)
{
    if (conn == nullptr)
    {
        std::cerr << "数据库未连接,无法执行SQL" << sql << std::endl;
        return false;
    }
    // 存储错误信息
    char *errMsg = nullptr;
    // 执行SQL,回调函数为nullptr，增删改不需要处理结果集
    int ret = sqlite3_exec(conn, sql.c_str(), nullptr, nullptr, &errMsg);
    if (ret != SQLITE_OK)
    {
        std::cerr << "SQL执行失败" << errMsg << ",SQL:" << sql << std::endl;
        sqlite3_free(errMsg); // 释放错误信息内存
        return false;
    }
    return true;
}

bool SqliteUtils::executeQuery(const std::string &sql, int (*callback)(void *, int, char **, char **))
{
    if (conn == nullptr)
    {
        std::cerr << "数据库未连接,无法执行SQL" << sql << std::endl;
        return false;
    }
    char *errMsg = nullptr; // 存储错误信息
    // 执行SQL，传入回调函数处理结果
    int ret = sqlite3_exec(conn, sql.c_str(), callback, nullptr, &errMsg);
    if (ret != SQLITE_OK)
    {
        std::cerr << "SQL查询失败:" << errMsg << ",SQL:" << sql << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

sqlite3 *SqliteUtils::getDbConnection()
{
    return conn;
}
