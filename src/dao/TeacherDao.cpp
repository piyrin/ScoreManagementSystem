#include"TeacherDao.h"
#include<sstream>
#include<iostream>


TeacherDao::TeacherDao()
{
    //通过SqliteUtils获取数据库连接
    this->db = SqliteUtils::getInstance()->getDbConnection();
    if(this->db==nullptr)
    {
        std::cerr << "TeacherDao初始化失败:数据库连接获取失败" << std::endl;
    }
    else
    {
        std::cout << "TeacherDao初始化成功:数据库连接已建立" << std ::endl;
    }
}

//析构函数无需手动关闭由SqliteUtils统一管理
TeacherDao::~TeacherDao()
{
}

bool TeacherDao::insert(const TeacherModel&teacher)
{
    if(this->db==nullptr)
        return false;
    //传入teacher，id自增无需传入
    std::string sql = "INSERT INTO teacher(teacherNo,name,gender,title,department,email)""VALUES(?,?,?,?,?,?)";

    //准备SQL语句
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if(ret!=SQLITE_OK)
    {
        std::cerr << "TeacherDao插入失败:SQL准备失败" << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    //绑定参数（占位符？对应的值，顺序与SQL字段一致）
    sqlite3_bind_text(stmt, 1, teacher.getTeacherNo().c_str(),-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, teacher.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, teacher.getGender().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, teacher.getTitle().c_str(),-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, teacher.getDepartment().c_str(),-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, teacher.getEmail().c_str(), -1, SQLITE_TRANSIENT);

    //执行SQL
    ret = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if(ret==SQLITE_DONE)
    {
        std::cout << "TeacherDao插入成功:" << teacher.toString() << std::endl;
        return true;
    }
    else
    {
        std::cerr << "TeacherDao插入失败:执行错误-" << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }
}

//根据id查询教师
TeacherModel TeacherDao::selectById(int id)
{
    if(this->db==nullptr)
        return TeacherModel();
    
    //SQL语句：根据id查询教师
    std::string sql= "SELECT id, teacherNo, name, gender, title, department, email FROM teacher WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if(ret!=SQLITE_OK)
    {
        std::cerr << "TeacherDao查询失败:SQL准备失败—" << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return TeacherModel();
    }

    //绑定id参数
    sqlite3_bind_int(stmt, 1, id);
    //执行查询并解析结果
    TeacherModel teacher;
    ret = sqlite3_step(stmt);
    if(ret==SQLITE_ROW)
    {
        // 从结果集提取字段，构造TeacherModel对象
        int dbId = sqlite3_column_int(stmt, 0);
        const char* tNo = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        const char* tName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        const char* tGender = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        const char* tTitle = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        const char* tDept = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        const char* tEmail = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));

        std::string teacherNo = tNo ? tNo : "";
        std::string name = tName ? tName : "";
        std::string gender = tGender ? tGender : "";
        std::string title = tTitle ? tTitle : "";
        std::string department = tDept ? tDept : "";
        std::string email = tEmail ? tEmail : "";

        teacher = TeacherModel(dbId, teacherNo, name, gender, title, department, email);
    }
    else
    {
        std::cout << "TeacherDao查询失败:未找到ID为" << id << "的教师" << std::endl;
    }
    sqlite3_finalize(stmt);
    return teacher;
}

//根据工号查询教师
TeacherModel TeacherDao ::selectByTeacherNo(const std::string&teacherNo)
{
    if(this->db==nullptr)
        return TeacherModel();
    
    //SQL语句：根据teacherNo查询教师
    std::string sql = "SELECT id, teacherNo, name, gender, title, department, email FROM teacher WHERE teacherNo = ?;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret!=SQLITE_OK)
    {
        std::cerr << "TeacherDao查询失败:SQL准备失败-" << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return TeacherModel();
    }

    //绑定工号参数
    sqlite3_bind_text(stmt, 1, teacherNo.c_str(), -1, SQLITE_TRANSIENT);

    //执行查询并解析结果
    TeacherModel teacher;
    ret = sqlite3_step(stmt);
    if (ret == SQLITE_ROW)
    {
        int dbId = sqlite3_column_int(stmt, 0);
        const char* tNo = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        const char* tName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        const char* tGender = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        const char* tTitle = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        const char* tDept = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        const char* tEmail = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));

        std::string dbTeacherNo = tNo ? tNo : "";
        std::string name = tName ? tName : "";
        std::string gender = tGender ? tGender : "";
        std::string title = tTitle ? tTitle : "";
        std::string department = tDept ? tDept : "";
        std::string email = tEmail ? tEmail : "";

        teacher = TeacherModel(dbId, dbTeacherNo, name, gender, title, department, email);
    }
    else
    {
        std::cout << "TeacherDao查询失败:未找到工号为" << teacherNo << "的教师" << std::endl;
    }

    sqlite3_finalize(stmt);
    return teacher;
}

//查询所有教师
std::vector<TeacherModel> TeacherDao::selectAll()
{
    std::vector<TeacherModel> teacherList;
    if(this->db==nullptr)
        return teacherList;
    
    //SQL语句:查询所有教师
    std::string sql = "SELECT id, teacherNo, name, gender, title, department, email FROM teacher;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if(ret!=SQLITE_OK)
    {
        std::cerr << "TeacherDao查询失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return teacherList;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int dbId = sqlite3_column_int(stmt, 0);
        const char* tNo = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        const char* tName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        const char* tGender = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        const char* tTitle = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        const char* tDept = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        const char* tEmail = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));

        std::string teacherNo = tNo ? tNo : "";
        std::string name = tName ? tName : "";
        std::string gender = tGender ? tGender : "";
        std::string title = tTitle ? tTitle : "";
        std::string department = tDept ? tDept : "";
        std::string email = tEmail ? tEmail : "";

        teacherList.emplace_back(dbId, teacherNo, name, gender, title, department, email);
    }

    sqlite3_finalize(stmt);
    std::cout << "TeacherDao查询成功:共找到" << teacherList.size() << "名教师" << std::endl;
    return teacherList;
}

//更新教师信息
bool TeacherDao::update(const TeacherModel&teacher)
{
    if(this->db==nullptr||teacher.getId()==0)
        return false;
    
    //SQL语句：根据更新教师信息（不更新工号）
    std::string sql = "UPDATE teacher SET name = ?, gender = ?, title = ?, department = ?, email = ? WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "TeacherDao更新失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    //绑定参数（顺序与SQL字段一致）
    sqlite3_bind_text(stmt, 1, teacher.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, teacher.getGender().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, teacher.getTitle().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, teacher.getDepartment().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, teacher.getEmail().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 6, teacher.getId());

    //执行SQL
    ret = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if(ret==SQLITE_DONE)
    {
        std::cout << "TeacherDao更新成功:" << teacher.toString() << std::endl;
        return true;
    }
    else
    {
        std::cerr << "TeacherDao更新失败:执行错误 - " << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }
}

//根据id删除教师
bool TeacherDao::deleteById(int id)
{
    if(this->db==nullptr)
        return false;
    
    //SQL语句：根据id删除教师
    std::string sql = "DELETE FROM teacher WHERE id = ?;";
    sqlite3_stmt *stmt;
    int ret = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (ret != SQLITE_OK)
    {
        std::cerr << "TeacherDao删除失败:SQL准备错误 - " << sqlite3_errmsg(this->db) << std::endl;
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
        std::cout << "TeacherDao删除成功:ID为" << id << "的教师" << std::endl;
        return true;
    }
    else
    {
        std::cerr << "TeacherDao删除失败:执行错误 - " << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }
}