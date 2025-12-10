#ifndef TEACHER_DAO_H
#define TEACHER_DAO_H

#include <vector>
#include "../core/Model/TeacherModel.h" // 依赖教师模型
#include "../utils/SqliteUtils.h"       // 依赖SQLite工具类（后续开发，先预留接口）

//教师数据访问对象
//功能：封装教师数据的数据库操作（增删改查）
//依赖SQLite数据库（bin/score_db.db）SqliteUtils工具类

class TeacherDao
{
public:
    //构造函数：初始化数据库连接，通SqliteUtils获取
    TeacherDao();

    //析构函数：释放资源
    ~TeacherDao();

    // 1. 新增教师（用于教师注册/管理员添加教师）
    // 参数：TeacherModel对象（无id，id由数据库自增）
    // 返回：true-成功，false-失败（如工号重复）
    bool insert(const TeacherModel &teacher);

    // 2. 根据ID查询教师（用于加载教师详情）
    // 参数：教师id（对应TeacherModel的id字段）
    // 返回：查询到的TeacherModel（id=0表示未查询到）
    TeacherModel selectById(int id);

    // 3. 根据工号查询教师（用于登录校验、工号唯一性校验）
    // 参数：教师工号（teacherNo）
    // 返回：查询到的TeacherModel（id=0表示未查询到）
    TeacherModel selectByTeacherNo(const std::string &teacherNo);

    // 4. 查询所有教师（用于管理员管理教师列表）
    // 返回：教师对象列表（std::vector<TeacherModel>）
    std ::vector<TeacherModel> selectAll();

    // 5. 更新教师信息（用于教师修改个人资料）
    // 参数：完整的TeacherModel对象（含id，根据id更新其他字段）
    // 返回：true-成功，false-失败（如id不存在）
    bool update(const TeacherModel &teacher);

    // 6. 根据ID删除教师（用于管理员删除教师）
    // 参数：教师id
    // 返回：true-成功，false-失败
    bool deleteById(int id);

private:
    //SQLite数据库连接句柄，通过SqliteUtils管理
    sqlite3 *db;//依赖SqliteUtils.h中定义的sqlite3类型
};