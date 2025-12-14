#ifndef STUDENT_DAO_H
#define STUDENT_DAO_H

#include <vector>
#include "../core/Model/StudentModel.h" // 依赖学生模型
#include "../utils/SqliteUtils.h"       // 依赖SQLite工具类

/**
 * 学生数据访问对象（新增文件）
 * 功能：封装学生数据的数据库操作（增删改查）
 * 依赖：SQLite数据库（bin/score_db.db）、SqliteUtils工具类、StudentModel模型
 */
class StudentDao
{
public:
    // 构造函数：初始化数据库连接（通过SqliteUtils获取）
    StudentDao();

    // 析构函数：释放资源（无需手动关连接，由SqliteUtils统一管理）
    ~StudentDao();

    // 1. 新增学生（学生注册/管理员添加学生）
    // 参数：StudentModel对象（无id，id由数据库自增）
    // 返回：true-成功，false-失败（如学号重复）
    bool insert(const StudentModel &student);

    // 2. 根据ID查询学生（加载学生详情）
    // 参数：学生id（对应StudentModel的id字段）
    // 返回：查询到的StudentModel（id=0表示未查到）
    StudentModel selectById(int id);

    // 3. 根据学号查询学生（登录校验/学号唯一性校验）
    // 参数：学生学号（studentNo，业务唯一）
    // 返回：查询到的StudentModel（id=0表示未查到）
    StudentModel selectByStudentNo(const std::string &studentNo);

    // 4. 查询所有学生（管理员管理学生列表）
    // 返回：学生对象列表
    std::vector<StudentModel> selectAll();

    // 5. 更新学生信息（学生修改个人资料）
    // 参数：完整的StudentModel对象（含id，根据id更新其他字段）
    // 返回：true-成功，false-失败（如id不存在）
    bool update(const StudentModel &student);

    // 6. 根据ID删除学生（管理员删除学生）
    // 参数：学生id
    // 返回：true-成功，false-失败
    bool deleteById(int id);

private:
    // SQLite数据库连接句柄（通过SqliteUtils获取）
    sqlite3 *db;
};

#endif // STUDENT_DAO_H