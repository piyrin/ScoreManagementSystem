#ifndef STUDENT_DAO_H
#define STUDENT_DAO_H

#include <vector>
#include "../core/Model/StudentModel.h" // 依赖学生模型
#include "../utils/SqliteUtils.h"       // 依赖SQLite工具类


//功能：封装学生数据的数据库操作（增删改查）

class StudentDao
{
public:
    //初始化数据库连接（通过SqliteUtils获取）
    StudentDao();

    //无需手动关连接，由SqliteUtils统一管理
    ~StudentDao();

    // 新增学生
    bool insert(const StudentModel &student);

    // 根据ID查询学生（加载学生详情）
    StudentModel selectById(int id);

    // 根据学号查询学生（登录校验/学号唯一性校验）
    StudentModel selectByStudentNo(const std::string &studentNo);

    // 查询所有学生（管理员管理学生列表）
    std::vector<StudentModel> selectAll();

    // 更新学生信息（学生修改个人资料）
    bool update(const StudentModel &student);

    // 根据ID删除学生（管理员删除学生）
    bool deleteById(int id);

private:
    // SQLite数据库连接句柄（通过SqliteUtils获取）
    sqlite3 *db;
};

#endif 