#ifndef STUDENT_DAO_H
#define STUDENT_DAO_H

#include <vector>
#include "../core/Model/StudentModel.h" 
#include "../utils/SqliteUtils.h"       

// 功能：封装学生数据的数据库操作

class StudentDao
{
public:
    // 初始化数据库连接
    StudentDao();
    ~StudentDao();

    // 新增学生
    bool insert(const StudentModel &student);

    // 根据ID查询学生
    StudentModel selectById(int id);

    // 根据学号查询学生
    StudentModel selectByStudentNo(const std::string &studentNo);

    // 更新学生信息
    bool update(const StudentModel &student);

    // 根据ID删除学生
    bool deleteById(int id);

private:
    // SQLite数据库连接句柄
    sqlite3 *db;
};

#endif