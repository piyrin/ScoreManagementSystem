#ifndef COURSE_DAO_H
#define COURSE_DAO_H

#include <vector>
#include "../core/Model/CourseModel.h"
#include "../utils/SqliteUtils.h"

/**
 * 课程数据访问对象（操作 course 表）
 * 核心功能：课程增删改查，支持按教师ID查询授课课程
 */
class CourseDao
{
public:
    CourseDao();
    ~CourseDao();

    //新增课程（管理员或教师添加）
    //返回：成功-true,失败-false
    bool insert(const CourseModel &course);

    //根据ID查课程
    CourseModel selectById(int id);

    //根据教师id查课程,教师查教的课
    std::vector<CourseModel> selectByTeacherId(int teacherId);

    //查询所有课程
    std::vector<CourseModel> selectAll();

    //更改课程信息（修改授课老师，学分等）
    bool update(const CourseModel &course);

    //根据id删除课程
    bool deleteById(int id);

private:
    sqlite3 *db;
};

#endif