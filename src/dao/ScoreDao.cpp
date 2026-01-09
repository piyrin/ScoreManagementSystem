#include "ScoreDao.h"
#include <iostream>
#include <vector>
#include <sqlite3.h>

ScoreDao::ScoreDao()
{
    this->db = SqliteUtils::getInstance()->getDbConnection();
    if (this->db == nullptr)
    {
        std::cerr << "ScoreDao初始化失败:数据库连接获取失败" << std::endl;
    }
    else
    {
        std::cout << "ScoreDao初始化成功:数据库连接已建立" << std::endl;
    }
}

ScoreDao::~ScoreDao()
{
}

// 1. 新增成绩
bool ScoreDao::insert(const ScoreModel &score)
{
    if (this->db == nullptr)
        return false;

    std::string sql = "INSERT INTO score (studentId, courseId, teacherId, score) VALUES (?, ?, ?, ?);";

    // 准备SQL语句
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "ScoreDao插入失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_bind_int(stmt, 1, score.getStudentId());
    sqlite3_bind_int(stmt, 2, score.getCourseId());
    sqlite3_bind_int(stmt, 3, score.getTeacherId());
    sqlite3_bind_double(stmt, 4, score.getScore());

    ret = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (ret == SQLITE_DONE)
    {
        std::cout << "ScoreDao插入成功:" << score.toString() << std::endl;
        return true;
    }
    else
    {
        std::cerr << "ScoreDao插入失败:执行错误（可能学生+课程重复）- " << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }
}

// 3. 根据学生ID查询成绩列表
std::vector<ScoreModel> ScoreDao::selectByStudentId(int studentId)
{
    std::vector<ScoreModel> scoreList;
    if (this->db == nullptr)
        return scoreList;

    // SQL语句：根据studentId查询该学生的所有成绩
    std::string sql = "SELECT id, studentId, courseId, teacherId, score FROM score WHERE studentId = ?;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "ScoreDao查询失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return scoreList;
    }

    // 绑定学生ID参数
    sqlite3_bind_int(stmt, 1, studentId);

    // 循环解析结果集
    while ((ret = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int dbId = sqlite3_column_int(stmt, 0);
        int dbStudentId = sqlite3_column_int(stmt, 1);
        int dbCourseId = sqlite3_column_int(stmt, 2);
        int dbTeacherId = sqlite3_column_int(stmt, 3);
        double scoreVal = sqlite3_column_double(stmt, 4);

        // 构造ScoreModel对象并加入列表
        ScoreModel score(dbId, dbStudentId, dbCourseId, scoreVal, dbTeacherId);
        scoreList.emplace_back(score);
    }

    sqlite3_finalize(stmt);
    std::cout << "ScoreDao查询成功:学生ID=" << studentId << "，共找到" << scoreList.size() << "条成绩" << std::endl;
    return scoreList;
}

// 4. 根据课程ID查询成绩列表
std::vector<ScoreModel> ScoreDao::selectByCourseId(int courseId)
{
    std::vector<ScoreModel> scoreList;
    if (this->db == nullptr)
        return scoreList;

    // SQL语句：根据courseId查询该课程的所有成绩
    std::string sql = "SELECT id, studentId, courseId, teacherId, score FROM score WHERE courseId = ?;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "ScoreDao查询失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return scoreList;
    }

    // 绑定课程ID参数
    sqlite3_bind_int(stmt, 1, courseId);

    // 循环解析结果集
    while ((ret = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int dbId = sqlite3_column_int(stmt, 0);
        int dbStudentId = sqlite3_column_int(stmt, 1);
        int dbCourseId = sqlite3_column_int(stmt, 2);
        int dbTeacherId = sqlite3_column_int(stmt, 3);
        double scoreVal = sqlite3_column_double(stmt, 4);

        // 构造ScoreModel对象并加入列表
        ScoreModel score(dbId, dbStudentId, dbCourseId, scoreVal, dbTeacherId);
        scoreList.emplace_back(score);
    }

    sqlite3_finalize(stmt);
    std::cout << "ScoreDao查询成功:课程ID=" << courseId << "，共找到" << scoreList.size() << "条成绩" << std::endl;
    return scoreList;
}

//  更新成绩
bool ScoreDao::update(const ScoreModel &score)
{
    if (this->db == nullptr || score.getId() == 0)
        return false;

    // SQL语句：根据id更新成绩信息（可更新分数、备注）
    std::string sql = "UPDATE score SET score = ? WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "ScoreDao更新失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // 绑定参数（顺序与SQL字段一致）
    sqlite3_bind_double(stmt, 1, score.getScore()); // 分数
    sqlite3_bind_int(stmt, 2, score.getId());       // 成绩ID（条件）

    // 执行SQL
    ret = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (ret == SQLITE_DONE)
    {
        std::cout << "ScoreDao更新成功:" << score.toString() << std::endl;
        return true;
    }
    else
    {
        std::cerr << "ScoreDao更新失败:执行错误 - " << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }
}

// 根据ID删除成绩
bool ScoreDao::deleteById(int id)
{
    if (this->db == nullptr)
        return false;

    // SQL语句：根据id删除成绩
    std::string sql = "DELETE FROM score WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "ScoreDao删除失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // 绑定id参数
    sqlite3_bind_int(stmt, 1, id);

    // 执行SQL
    ret = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (ret == SQLITE_DONE)
    {
        std::cout << "ScoreDao删除成功:ID为" << id << "的成绩" << std::endl;
        return true;
    }
    else
    {
        std::cerr << "ScoreDao删除失败:执行错误 - " << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }
}

std::vector<ScoreModel> ScoreDao::selectByStudentIdAndCourseId(int studentId, int courseId)
{
    std::vector<ScoreModel> scoreList;
    if (this->db == nullptr)
        return scoreList;

    // SQL：查询指定学生+课程的成绩
    std::string sql = "SELECT id, studentId, courseId, teacherId, score FROM score WHERE studentId = ? AND courseId = ?;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "ScoreDao查询失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return scoreList;
    }

    // 绑定参数
    sqlite3_bind_int(stmt, 1, studentId);
    sqlite3_bind_int(stmt, 2, courseId);

    // 得到结果
    while ((ret = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int dbId = sqlite3_column_int(stmt, 0);
        int dbStudentId = sqlite3_column_int(stmt, 1);
        int dbCourseId = sqlite3_column_int(stmt, 2);
        int dbTeacherId = sqlite3_column_int(stmt, 3);
        double scoreVal = sqlite3_column_double(stmt, 4);

        scoreList.emplace_back(dbId, dbStudentId, dbCourseId, scoreVal, dbTeacherId);
    }

    sqlite3_finalize(stmt);
    return scoreList;
}
