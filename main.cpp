#include <iostream>
#include "service/AuthService.h"
#include "service/StudentService.h"
#include "core/Model/StudentModel.h"
#include "core/Model/TeacherModel.h"

// 打印学生业务操作结果（辅助函数）
void printStudentOpResult(StudentOpResult result)
{
    switch (result)
    {
    case StudentOpResult::SUCCESS:
        std::cout << "操作成功！" << std::endl;
        break;
    case StudentOpResult::ROLE_ERROR:
        std::cout << "操作失败：角色错误！" << std::endl;
        break;
    case StudentOpResult::PARAM_ERROR:
        std::cout << "操作失败：参数错误！" << std::endl;
        break;
    case StudentOpResult::NOT_FOUND:
        std::cout << "操作失败：未找到相关数据！" << std::endl;
        break;
    case StudentOpResult::SYSTEM_ERROR:
        std::cout << "操作失败：系统错误！" << std::endl;
        break;
    }
}

int main()
{
    // 1. 初始化服务
    AuthService auth_service;
    StudentService student_service;
    UserModel login_user;
    LoginResult login_res;

    // 2. 学生登录（使用之前注册的学生账号）
    std::cout << "\n===== 学生登录 =====" << std::endl;
    login_res = auth_service.login("zhangsan", "654321", login_user);
    if (login_res != LoginResult::SUCCESS)
    {
        std::cout << "登录失败，无法执行学生业务！" << std::endl;
        return 1;
    }

    // 3. 查询个人信息
    std::cout << "\n===== 查询个人信息 =====" << std::endl;
    StudentModel my_info = student_service.getStudentInfo(login_user);
    if (my_info.getId() == 0)
    {
        std::cout << "个人信息查询失败！" << std::endl;
        return 1;
    }
    else
    {
        std::cout << "个人信息：" << my_info.toString() << std::endl;
    }

    // 4. 修改个人信息（示例：修改姓名、专业、电话、年级，不修改性别）
    std::cout << "\n===== 修改个人信息 =====" << std::endl;
    StudentModel new_info = my_info;
    new_info.setName("张三三");       // 新姓名
    new_info.setMajor("人工智能");    // 新专业
    new_info.setGrade("2024级");      // 新年级
    new_info.setPhone("13900139000"); // 新电话
    // 不修改性别：即使new_info设置了性别，也不会生效（保留原始性别）
    StudentOpResult update_res = student_service.updateStudentInfo(login_user, new_info);
    printStudentOpResult(update_res);

    // 5. 重新查询个人信息（验证修改结果）
    std::cout << "\n===== 重新查询个人信息 =====" << std::endl;
    StudentModel updated_info = student_service.getStudentInfo(login_user);
    std::cout << "修改后信息：" << updated_info.toString() << std::endl;

    // 6. 查询个人成绩（测试数据需手动插入数据库）
    std::cout << "\n===== 查询个人成绩 =====" << std::endl;
    std::vector<ScoreModel> my_scores = student_service.getMyScores(login_user);

    return 0;
}