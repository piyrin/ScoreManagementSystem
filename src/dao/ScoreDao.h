#ifndef SCORE_DAO_H
#define SCORE_DAO_H

#include <vector>
#include "../core/Model/ScoreModel.h" // 依赖成绩模型
#include "../utils/SqliteUtils.h"     // 依赖SQLite工具类

/**
 * 成绩数据访问对象（新增文件）
 * 功能：封装成绩数据的数据库操作（增删改查）
 * 依赖：SQLite数据库、SqliteUtils工具类、ScoreModel模型
 * 核心关联：studentId→student表、courseId→course表、teacherId→teacher表
 */
class ScoreDao
{
public:
    // 构造函数：初始化数据库连接（通过SqliteUtils获取）
    ScoreDao();

    // 析构函数：释放资源（无需手动关连接，由SqliteUtils统一管理）
    ~ScoreDao();

    // 1. 新增成绩（教师录入成绩）
    // 参数：ScoreModel对象（无id，id由数据库自增）
    // 返回：true-成功，false-失败（如学生+课程重复、外键不存在）
    bool insert(const ScoreModel &score);

    // 2. 根据ID查询成绩（查看单条成绩详情）
    // 参数：成绩id（对应ScoreModel的id字段）
    // 返回：查询到的ScoreModel（id=0表示未查到）
    ScoreModel selectById(int id);

    // 3. 根据学生ID查询成绩列表（学生查看自己的所有成绩）
    // 参数：学生id（对应StudentModel的id）
    // 返回：该学生的所有成绩列表
    std::vector<ScoreModel> selectByStudentId(int studentId);

    // 4. 根据课程ID查询成绩列表（教师查看自己课程的所有学生成绩）
    // 参数：课程id（对应CourseModel的id）
    // 返回：该课程的所有学生成绩列表
    std::vector<ScoreModel> selectByCourseId(int courseId);

    // 5. 查询所有成绩（管理员统计/管理所有成绩）
    // 返回：所有成绩对象列表
    std::vector<ScoreModel> selectAll();

    // 6. 更新成绩（教师修改成绩）
    // 参数：完整的ScoreModel对象（含id，根据id更新其他字段）
    // 返回：true-成功，false-失败（如id不存在）
    bool update(const ScoreModel &score);

    // 7. 根据ID删除成绩（管理员/教师删除错误成绩）
    // 参数：成绩id
    // 返回：true-成功，false-失败
    bool deleteById(int id);

    // 根据学生ID和课程ID查询成绩（校验重复）
    std::vector<ScoreModel> selectByStudentIdAndCourseId(int studentId, int courseId);

private:
    // SQLite数据库连接句柄（通过SqliteUtils获取）
    sqlite3 *db;
};

#endif // SCORE_DAO_H