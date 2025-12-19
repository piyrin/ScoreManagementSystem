#include "CourseDao.h"
#include <iostream>
#include <vector>
#include <sqlite3.h>

// 构造函数，初始化数据库连接
CourseDao::CourseDao()
{
    // 通过SqliteUtils单例获取数据库连接
    this->db = SqliteUtils::getInstance()->getDbConnection();
    if (this->db == nullptr)
    {
        std::cerr << "CourseDao初始化失败:数据库连接获取失败" << std::endl;
    }
    else
    {
        std::cout << "CourseDao初始化成功:数据库连接已建立" << std::endl;
    }
}

// 析构函数，释放资源（无需关链接）
CourseDao::~CourseDao()
{
    std::cout << "CouresDao资源释放" << std::endl;
}

bool CourseDao::insert(const CourseModel &course)
{
    if (this->db == nullptr)
        return false;

    // SQL语句：插入成绩数据（id自增，无需传入）
    std::string sql = "INSERT INTO course(courseNo,courseName,credit,teacherId,description)"
                      "VALUES(?,?,?,?,?)";

    // 准备SQL语句
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "CourseDao插入失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // 绑定参数（顺序与SQL字段一致）
    sqlite3_bind_text(stmt, 1, course.getCourseNo().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, course.getCourseName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, course.getCredit());
    sqlite3_bind_int(stmt, 4, course.getTeacherId());
    sqlite3_bind_text(stmt, 5, course.getDescription().c_str(), -1, SQLITE_TRANSIENT);

    // 执行SQL
    ret = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (ret == SQLITE_DONE)
    {
        std::cout << "CourseDao插入成功:" << course.toString() << std::endl;
        return true;
    }
    else
    {
        std::cerr << "CourseDao插入失败:执行错误(可能课程号重复)- " << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }
}

// 根据id查课程
CourseModel CourseDao::selectById(int id)
{
    if (this->db == nullptr)
        return CourseModel();
    // SQL语句：根据id查课程
    std::string sql = "SELECT id, courseNo, courseName, credit, teacherId, description FROM course WHERE id=?;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "CourseDao查询失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return CourseModel();
    }

    // 绑定id参数
    sqlite3_bind_int(stmt, 1, id);

    // 执行查询并解析结果
    CourseModel course;
    ret = sqlite3_step(stmt);
    if (ret == SQLITE_ROW)
    {
        int dbid = sqlite3_column_int(stmt, 0);
        std::string courseNo = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::string courseName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        int credit = sqlite3_column_int(stmt, 3);
        int teacherId = sqlite3_column_int(stmt, 4);
        std::string description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        course = CourseModel(dbid, courseNo, courseName, credit, teacherId, description);
    }
    else
    {
        std::cout << "CourseDao查询失败:未找到ID为" << id << "的课程" << std::endl;
    }
    sqlite3_finalize(stmt);
    return course;
}

std::vector<CourseModel> CourseDao::selectByTeacherId(int teacherId)
{
    std::vector<CourseModel> courseList;
    if (this->db == nullptr)
    {
        return courseList;
    }

    // SQL语句：根据teacherId查询教授课程
    std::string sql = "SELECT id, courseNo, courseName, credit, teacherId, description FROM course WHERE teacherId = ?;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "CourseDao查询失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return courseList;
    }

    // 绑定老师ID参数
    sqlite3_bind_int(stmt, 1, teacherId);

    // 循环解析结果集
    while ((ret = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int dbId = sqlite3_column_int(stmt, 0);
        std::string courseNo = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::string courseName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        int credit = sqlite3_column_int(stmt, 3);
        int dbTeacherId = sqlite3_column_int(stmt, 4);
        std::string description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));

        courseList.emplace_back(dbId, courseNo, courseName, credit, dbTeacherId, description);
    }

    sqlite3_finalize(stmt);
    std::cout << "CourseDao查询成功:教师ID=" << teacherId << "，共找到" << courseList.size() << "门课程" << std::endl;
    return courseList;
}

// 4. 查询所有课程
std::vector<CourseModel> CourseDao::selectAll()
{
    std::vector<CourseModel> courseList;
    if (this->db == nullptr)
        return courseList;

    std::string sql = "SELECT id, courseNo, courseName, credit, teacherId, description FROM course;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "CourseDao查询失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return courseList;
    }

    // 解析结果集
    while ((ret = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int dbId = sqlite3_column_int(stmt, 0);
        std::string courseNo = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::string courseName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        int credit = sqlite3_column_int(stmt, 3);
        int teacherId = sqlite3_column_int(stmt, 4);
        std::string description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));

        courseList.emplace_back(dbId, courseNo, courseName, credit, teacherId, description);
    }

    sqlite3_finalize(stmt);
    std::cout << "CourseDao查询成功:共找到" << courseList.size() << "门课程" << std::endl;
    return courseList;
}

// 5. 更新课程信息
bool CourseDao::update(const CourseModel &course)
{
    if (this->db == nullptr || course.getId() == 0)
        return false;

    // 校验必填参数
    if (course.getCourseNo().empty() || course.getCourseName().empty() || course.getCredit() <= 0 || course.getTeacherId() == 0)
    {
        std::cerr << "CourseDao更新失败:必填参数无效" << std::endl;
        return false;
    }

    // SQL语句：更新课程信息（课程号不可修改，避免冲突）
    std::string sql = "UPDATE course SET courseName = ?, credit = ?, teacherId = ?, description = ? WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "CourseDao更新失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // 绑定参数（课程号不允许修改，仅更新其他字段）
    sqlite3_bind_text(stmt, 1, course.getCourseName().c_str(), -1, SQLITE_TRANSIENT);  // 课程名
    sqlite3_bind_int(stmt, 2, course.getCredit());                                     // 学分
    sqlite3_bind_int(stmt, 3, course.getTeacherId());                                  // 教师ID
    sqlite3_bind_text(stmt, 4, course.getDescription().c_str(), -1, SQLITE_TRANSIENT); // 描述
    sqlite3_bind_int(stmt, 5, course.getId());                                         // 课程ID（条件）

    // 执行SQL
    ret = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (ret == SQLITE_DONE)
    {
        std::cout << "CourseDao更新成功:" << course.toString() << std::endl;
        return true;
    }
    else
    {
        std::cerr << "CourseDao更新失败:执行错误 - " << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }
}

// 6. 根据ID删除课程
bool CourseDao::deleteById(int id)
{
    if (this->db == nullptr || id == 0)
        return false;

    std::string sql = "DELETE FROM course WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "CourseDao删除失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // 绑定ID参数
    sqlite3_bind_int(stmt, 1, id);

    // 执行SQL
    ret = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (ret == SQLITE_DONE)
    {
        std::cout << "CourseDao删除成功:ID为" << id << "的课程" << std::endl;
        return true;
    }
    else
    {
        std::cerr << "CourseDao删除失败:执行错误 - " << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }
}