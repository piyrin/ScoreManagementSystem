#ifndef SQLITEUTILS_H
#define SQLITEUTILS_H
#include <string>
// 关联配置模块，获取相关配置
#include "../config/Config.h"

// 声明sqlite连接句柄
struct sqlite3;

class SqlitUtils
{
private:
    // 单例指针
    static SqlitUntils *instance;
    // sqlite数据库连接句柄
    sqlite3 *conn;

    // 私有构造函数：初始化数据库连接，从config获取数据
    SqlitUtils();
    // 私有析构函数：关闭数据库连接
    ~SqlitUtils();

    // 禁止拷贝构造和赋值
    SqlitUtils(const SqlitUntils &) = delete;
    SqlitUtils &operator=(const SqlitUntils &) = delete;

public:
    // 公有静态函数，获取实例指针
    static SqlitUtils *getInstance();

    // 核心接口1：执行[增/删/改]sql(如插入用户，修改成绩)
    // 返回值：true=成功，false=失败
    bool executeUpdate(const std::string &sql);

    // 核心接口2：查询sql（如查成绩，查用户信息）
    // callback：处理查询结果的回调函数
    bool executeQuery(const std::string &sql, int (*callback)(void *, int, char **, char **));

    // 辅助接口：获取数据库连接句柄
    sqlite3 *getDbConnection();
};
#endif