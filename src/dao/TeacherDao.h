#ifndef TEACHER_DAO_H
#define TEACHER_DAO_H

#include <vector>
#include "../core/Model/TeacherModel.h" // 依赖教师模型
#include "../utils/SqliteUtils.h"       // 依赖SQLite工具类（后续开发，先预留接口）


//功能：封装教师数据的数据库操作（增删改查）


class TeacherDao
{
public:
    //初始化数据库连接，通SqliteUtils获取
    TeacherDao();

    ~TeacherDao();

    // 新增教师（用于教师注册/管理员添加教师）
    bool insert(const TeacherModel &teacher);

    //根据ID查询教师（用于加载教师详情）
    TeacherModel selectById(int id);

    //根据工号查询教师（用于登录校验、工号唯一性校验）
    TeacherModel selectByTeacherNo(const std::string &teacherNo);

    //查询所有教师（用于管理员管理教师列表）
    std ::vector<TeacherModel> selectAll();

    // 更新教师信息（用于教师修改个人资料）
    bool update(const TeacherModel &teacher);

    // 根据ID删除教师（用于管理员删除教师）
    bool deleteById(int id);

private:
    //SQLite数据库连接句柄，通过SqliteUtils管理
    sqlite3 *db;
#endif