#ifndef SQLITEUTILS_H
#define SQLITEUTILS_H
#include <string>
#include <sqlite3.h>
// 关联配置模块，获取相关配置
#include "../config/Config.h"

// 声明sqlite连接句柄
struct sqlite3;

class SqliteUtils
{
private:
    static SqliteUtils *instance;
    // sqlite数据库连接句柄
    sqlite3 *conn;

    //初始化数据库连接，从config获取数据
    SqliteUtils();
    //关闭数据库连接
    ~SqliteUtils();
    std::string dbPath; // 数据库文件路径（bin/score_db.db）

    SqliteUtils(const SqliteUtils &) = delete;
    SqliteUtils &operator=(const SqliteUtils &) = delete;

    //建表
    bool createAllTables();

public:
    //获取实例指针
    static SqliteUtils *getInstance();

    //执行[增/删/改]sql(如插入用户，修改成绩)
    bool executeUpdate(const std::string &sql);

    //查询sql（如查成绩，查用户信息）
    bool executeQuery(const std::string &sql, int (*callback)(void *, int, char **, char **));

    // 辅助接口：获取数据库连接句柄
    sqlite3 *getDbConnection();

    // 检查并创建默认管理员
    void checkAndCreateDefaultAdmin();
};

#endif
