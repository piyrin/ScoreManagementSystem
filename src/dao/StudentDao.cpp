#include "StudentDao.h"
#include <sstream>
#include <iostream>

StudentDao::StudentDao()
{
    // 通过SqliteUtils获取数据库连接
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

// 析构函数无需手动关闭由SqliteUtils统一管理
StudentDao::~StudentDao()
{
    std::cout << "StudentDao资源释放" << std::endl;
}

bool StudentDao::insert(const StudentModel&student)
{
    if(this->db==nullptr)
        return false;
    std::string sql = "INSERT INTO student (studentNo, name, gender, major, grade, phone) " "VALUES (?, ?, ?, ?, ?, ?);";
    // 准备SQL语句
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "StudentDao插入失败:SQL准备失败" << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // 绑定参数（顺序与SQL字段一致）
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
        std::cout << "StudentDao插入成功:" << teacher.toString() << std::endl;
        return true;
    }
    else
    {
        std::cerr << "StudentDao插入失败:执行错误-" << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }
}

//根据id查学生
StudentModel StudentDao::selectById(int id)
{
    if (this->db == nullptr)
        return TeacherModel();
    // SQL语句：根据id查询学生
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

    // 执行查询并解析结果
    StudentModel student;
    ret = sqlite3_step(stmt);
    if (ret == SQLITE_ROW)
    {
        // 从结果集提取字段（顺序与SQL查询字段一致）
        int dbId = sqlite3_column_int(stmt, 0);
        std::string studentNo = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::string name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        std::string gender = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        std::string major = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        std::string grade = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        std::string phone = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));

        // 构造StudentModel对象
        student = StudentModel(dbId, studentNo, name, gender, major, grade, phone);
    }
    else
    {
        std::cout << "StudentDao查询失败:未找到ID为" << id << "的学生" << std::endl;
    }

    sqlite3_finalize(stmt);
    return student;
}

// 3. 根据学号查询学生
StudentModel StudentDao::selectByStudentNo(const std::string &studentNo)
{
    if (this->db == nullptr)
        return StudentModel();

    // SQL语句：根据学号查询学生（学号唯一）
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
        std::string dbStudentNo = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::string name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        std::string gender = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        std::string major = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        std::string grade = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        std::string phone = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));

        student = StudentModel(dbId, dbStudentNo, name, gender, major, grade, phone);
    }
    else
    {
        std::cout << "StudentDao查询失败:未找到学号为" << studentNo << "的学生" << std::endl;
    }

    sqlite3_finalize(stmt);
    return student;
}

// 4. 查询所有学生
std::vector<StudentModel> StudentDao::selectAll()
{
    std::vector<StudentModel> studentList;
    if (this->db == nullptr)
        return studentList;

    // SQL语句：查询所有学生
    std::string sql = "SELECT id, studentNo, name, gender, major, grade, phone FROM student;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "StudentDao查询失败：SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return studentList;
    }

    // 循环解析结果集（直到没有更多行）
    while ((ret = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int dbId = sqlite3_column_int(stmt, 0);
        std::string studentNo = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::string name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        std::string gender = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        std::string major = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        std::string grade = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        std::string phone = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));

        // 直接构造对象加入列表（emplace_back效率更高）
        studentList.emplace_back(dbId, studentNo, name, gender, major, grade, phone);
    }

    sqlite3_finalize(stmt);
    std::cout << "StudentDao查询成功：共找到" << studentList.size() << "名学生" << std::endl;
    return studentList;
}

// 5. 更新学生信息（不更新学号，学号唯一不可修改）
bool StudentDao::update(const StudentModel &student)
{
    if (this->db == nullptr || student.getId() == 0)
        return false;

    // SQL语句：根据id更新学生信息
    std::string sql = "UPDATE student SET name = ?, gender = ?, major = ?, grade = ?, phone = ? WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "StudentDao更新失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // 绑定参数（顺序与SQL字段一致）
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

// 6. 根据ID删除学生
bool StudentDao::deleteById(int id)
{
    if (this->db == nullptr)
        return false;

    // SQL语句：根据id删除学生
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