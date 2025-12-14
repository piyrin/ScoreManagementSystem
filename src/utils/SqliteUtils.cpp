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

// 实现自动建表方法（按依赖顺序建表，无外键→有外键）
bool SqliteUtils::createAllTables()
{
    if (conn == nullptr)
    {
        std::cerr << "数据库未连接，无法建表" << std::endl;
        return false;
    }

    // 1. 建user表（用户登录表，无外键）
    std::string createUserTable = R"(
        CREATE TABLE IF NOT EXISTS user (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,  -- 登录用户名唯一（学生/教师/管理员共用）
            password TEXT NOT NULL,         -- MD5加密后的密码
            role INTEGER NOT NULL,          -- 角色:1=管理员,2=教师,3=学生
            relatedId INTEGER NOT NULL,     -- 关联student.id(角色3)或teacher.id(角色2),管理员填1
            FOREIGN KEY (relatedId) REFERENCES student(id) ON DELETE SET NULL,
            FOREIGN KEY (relatedId) REFERENCES teacher(id) ON DELETE SET NULL
        );
    )";

    // 2. 建student表（学生信息表，无外键）
    std::string createStudentTable = R"(
        CREATE TABLE IF NOT EXISTS student (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            studentNo TEXT NOT NULL UNIQUE,  -- 学号唯一
            name TEXT NOT NULL,              -- 姓名
            gender TEXT,                     -- 性别
            major TEXT,                      -- 专业
            grade TEXT,                      -- 年级(如2025级)
            phone TEXT                       -- 联系电话
        );
    )";

    // 3. 建teacher表（教师信息表，无外键）
    std::string createTeacherTable = R"(
        CREATE TABLE IF NOT EXISTS teacher (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            teacherNo TEXT NOT NULL UNIQUE,  -- 工号唯一
            name TEXT NOT NULL,              -- 姓名
            gender TEXT,                     -- 性别
            title TEXT,                      -- 职称（教授/副教授）
            department TEXT,                 -- 所属部门
            email TEXT                       -- 邮箱（替换原电话）
        );
    )";

    // 4. 建course表（课程表，依赖teacher表外键）
    std::string createCourseTable = R"(
        CREATE TABLE IF NOT EXISTS course (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            courseNo TEXT NOT NULL UNIQUE,   -- 课程号唯一
            courseName TEXT NOT NULL,        -- 课程名称
            credit INTEGER NOT NULL,         -- 学分(对应ScoreModel绩点计算)
            teacherId INTEGER,               -- 关联授课教师id(teacher表)
            description TEXT,                -- 课程描述
            FOREIGN KEY (teacherId) REFERENCES teacher(id) ON DELETE SET NULL
        );
    )";

    // 5. 建score表（成绩表，依赖学生、课程、教师外键）
    std::string createScoreTable = R"(
        CREATE TABLE IF NOT EXISTS score (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            studentId INTEGER NOT NULL,      -- 关联学生id(student表)
            courseId INTEGER NOT NULL,       -- 关联课程id(course表)
            teacherId INTEGER NOT NULL,      -- 关联授课教师id(teacher表)
            score REAL NOT NULL,             -- 分数(支持小数,如85.5)
            examTime TEXT NOT NULL,          -- 考试时间(格式YYYY-MM-DD,对应ScoreModel)
            -- 外键约束:删除关联数据时,成绩记录保留(可根据需求调整为ON DELETE CASCADE)
            FOREIGN KEY (studentId) REFERENCES student(id) ON DELETE RESTRICT,
            FOREIGN KEY (courseId) REFERENCES course(id) ON DELETE RESTRICT,
            FOREIGN KEY (teacherId) REFERENCES teacher(id) ON DELETE RESTRICT,
            -- 联合唯一：一个学生一门课程只能有一条成绩记录
            UNIQUE (studentId, courseId)
        );
    )";

    // 执行建表（按顺序执行，失败则返回false）
    bool ret = true;
    ret &= executeUpdate(createUserTable);    // 先建user表
    ret &= executeUpdate(createStudentTable); // 再建student表
    ret &= executeUpdate(createTeacherTable); // 再建teacher表
    ret &= executeUpdate(createCourseTable);  // 再建course表（依赖teacher）
    ret &= executeUpdate(createScoreTable);   // 最后建score表（依赖前三者）

    return ret;
}