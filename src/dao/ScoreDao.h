#ifndef SCORE_DAO_H
#define SCORE_DAO_H

#include <vector>
#include "../core/Model/ScoreModel.h" // 依赖成绩模型
#include "../utils/SqliteUtils.h"     // 依赖SQLite工具类


//功能：封装成绩数据的数据库操作（增删改查）
class ScoreDao
{
public:
    // 初始化数据库连接
    ScoreDao();

    ~ScoreDao();

    // 新增成绩，教师录入成绩
    bool insert(const ScoreModel &score);

    // 根据ID查询成绩
    ScoreModel selectById(int id);

    // 根据学生ID查询成绩列表（学生查看自己的所有成绩）
    std::vector<ScoreModel> selectByStudentId(int studentId);

    // 根据课程ID查询成绩列表（教师查看自己课程的所有学生成绩）
    std::vector<ScoreModel> selectByCourseId(int courseId);

    //查询所有成绩（管理员统计/管理所有成绩）
    std::vector<ScoreModel> selectAll();

    // 更新成绩（教师修改成绩）
    bool update(const ScoreModel &score);

    // 根据ID删除成绩（管理员/教师删除错误成绩）
    bool deleteById(int id);

    // 根据学生ID和课程ID查询成绩（校验重复）
    std::vector<ScoreModel> selectByStudentIdAndCourseId(int studentId, int courseId);

private:
    // SQLite数据库连接句柄（通过SqliteUtils获取）
    sqlite3 *db;
};

#endif 