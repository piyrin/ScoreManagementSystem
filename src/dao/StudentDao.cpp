#include "StudentDao.h"
#include <sstream>
#include <iostream>

StudentDao::StudentDao()
{
    this->db = SqliteUtils::getInstance()->getDbConnection();
    if (this->db == nullptr)
    {
        std::cerr << "StudentDao初始化失败:数据库连接获取失败" << std::endl;
    }
    else
    {
        std::cout << "StudentDao初始化成功:数据库连接已建立" << std ::endl;
    }
}

StudentDao::~StudentDao()
{
}

bool StudentDao::insert(const StudentModel &student)
{
    if (this->db == nullptr)
        return false;
    std::string sql = "INSERT INTO student (studentNo, name, gender, major, grade, phone) VALUES (?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "StudentDao插入失败:SQL准备失败" << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // 绑定参数
    sqlite3_bind_text(stmt, 1, student.getStudentNo().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, student.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, student.getGender().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, student.getMajor().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, student.getGrade().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, student.getPhone().c_str(), -1, SQLITE_TRANSIENT);

    // 执行SQL
    ret = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (ret == SQLITE_DONE)
    {
        std::cout << "StudentDao插入成功:" << student.toString() << std::endl;
        return true;
    }
    else
    {
        std::cerr << "StudentDao插入失败:执行错误-" << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }
}

// 根据id查学生
StudentModel StudentDao::selectById(int id)
{
    if (this->db == nullptr)
        return StudentModel();
    //根据id查询学生
    std::string sql = "SELECT id, studentNo, name, gender, major, grade, phone FROM student WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "StudentDao查询失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return StudentModel();
    }

    // 绑定id参数
    sqlite3_bind_int(stmt, 1, id);

    StudentModel student;
    ret = sqlite3_step(stmt);
    if (ret == SQLITE_ROW)
    {
        int dbId = sqlite3_column_int(stmt, 0);
        const char *sNo = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        const char *sName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        const char *sGender = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        const char *sMajor = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        const char *sGrade = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        const char *sPhone = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));

        std::string studentNo = sNo ? sNo : "";
        std::string name = sName ? sName : "";
        std::string gender = sGender ? sGender : "";
        std::string major = sMajor ? sMajor : "";
        std::string grade = sGrade ? sGrade : "";
        std::string phone = sPhone ? sPhone : "";

        student = StudentModel(dbId, studentNo, name, gender, major, grade, phone);
    }
    else
    {
        std::cout << "StudentDao查询失败:未找到ID为" << id << "的学生" << std::endl;
    }

    sqlite3_finalize(stmt);
    return student;
}

//根据学号查询学生
StudentModel StudentDao::selectByStudentNo(const std::string &studentNo)
{
    if (this->db == nullptr)
        return StudentModel();

    //根据学号查询学生
    std::string sql = "SELECT id, studentNo, name, gender, major, grade, phone FROM student WHERE studentNo = ?;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "StudentDao查询失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return StudentModel();
    }

    // 绑定学号参数
    sqlite3_bind_text(stmt, 1, studentNo.c_str(), -1, SQLITE_TRANSIENT);

    // 执行查询并解析结果
    StudentModel student;
    ret = sqlite3_step(stmt);
    if (ret == SQLITE_ROW)
    {
        int dbId = sqlite3_column_int(stmt, 0);
        const char *sNo = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        const char *sName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        const char *sGender = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        const char *sMajor = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        const char *sGrade = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        const char *sPhone = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));

        std::string dbStudentNo = sNo ? sNo : "";
        std::string name = sName ? sName : "";
        std::string gender = sGender ? sGender : "";
        std::string major = sMajor ? sMajor : "";
        std::string grade = sGrade ? sGrade : "";
        std::string phone = sPhone ? sPhone : "";

        student = StudentModel(dbId, dbStudentNo, name, gender, major, grade, phone);
    }
    else
    {
        std::cout << "StudentDao查询失败:未找到学号为" << studentNo << "的学生" << std::endl;
    }

    sqlite3_finalize(stmt);
    return student;
}

//更新学生信息
bool StudentDao::update(const StudentModel &student)
{
    if (this->db == nullptr || student.getId() == 0)
        return false;

    //根据id更新学生信息
    std::string sql = "UPDATE student SET name = ?, gender = ?, major = ?, grade = ?, phone = ? WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "StudentDao更新失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // 绑定参数
    sqlite3_bind_text(stmt, 1, student.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, student.getGender().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, student.getMajor().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, student.getGrade().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, student.getPhone().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 6, student.getId());

    // 执行SQL
    ret = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (ret == SQLITE_DONE)
    {
        std::cout << "StudentDao更新成功:" << student.toString() << std::endl;
        return true;
    }
    else
    {
        std::cerr << "StudentDao更新失败:执行错误 - " << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }
}

//根据ID删除学生
bool StudentDao::deleteById(int id)
{
    if (this->db == nullptr)
        return false;

    //根据id删除学生
    std::string sql = "DELETE FROM student WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "StudentDao删除失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
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
        std::cout << "StudentDao删除成功:ID为" << id << "的学生" << std::endl;
        return true;
    }
    else
    {
        std::cerr << "StudentDao删除失败:执行错误 - " << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }
}