#ifndef SCORE_DAO_H
#define SCORE_DAO_H

#include <vector>
#include "../core/Model/ScoreModel.h" 
#include "../utils/SqliteUtils.h"    

// 功能：封装成绩数据的数据库操作
class ScoreDao
{
public:
    // 初始化数据库连接
    ScoreDao();

    ~ScoreDao();

    // 新增成绩，教师录入成绩
    bool insert(const ScoreModel &score);

    // 根据学生ID查询成绩列表
    std::vector<ScoreModel> selectByStudentId(int studentId);

    // 根据课程ID查询成绩列表
    std::vector<ScoreModel> selectByCourseId(int courseId);

    // 更新成绩
    bool update(const ScoreModel &score);

    // 根据ID删除成绩
    bool deleteById(int id);

    // 根据学生ID和课程ID查询成绩
    std::vector<ScoreModel> selectByStudentIdAndCourseId(int studentId, int courseId);

private:
    // SQLite数据库连接句柄
    sqlite3 *db;
};

#endif