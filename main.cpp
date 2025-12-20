#include <iostream>
#include <windows.h>
#include "service/AuthService.h"
#include "service/StudentService.h"
#include "core/Model/StudentModel.h"
#include "core/Model/TeacherModel.h"

void printStudentOpResult(StudentOpResult result)
{
    switch (result)
    {
    case StudentOpResult::SUCCESS:
        std::cout << "操作成功！" << std::endl;
        break;
    case StudentOpResult::ROLE_ERROR:
        std::cout << "Error: 角色权限不符" << std::endl;
        break;
    case StudentOpResult::PARAM_ERROR:
        std::cout << "Error: 参数校验失败" << std::endl;
        break;
    case StudentOpResult::NOT_FOUND:
        std::cout << "Error: 数据不存在" << std::endl;
        break;
    case StudentOpResult::SYSTEM_ERROR:
        std::cout << "Error: 系统内部错误" << std::endl;
        break;
    }
}

int main()
{
    SetConsoleOutputCP(65001);

    AuthService auth_service;
    StudentService student_service;
    UserModel login_user;

    std::cout << "\n===== 学生登录 =====" << std::endl;
    // 模拟登录操作
    auto login_res = auth_service.login("zhangsan", "654321", login_user);
    if (login_res != LoginResult::SUCCESS)
    {
        std::cerr << "[Login Failed] 无法执行后续业务" << std::endl;
        return 1;
    }

    std::cout << "\n===== 查询个人信息 =====" << std::endl;
    auto my_info = student_service.getStudentInfo(login_user);
    if (my_info.getId() == 0)
    {
        std::cerr << "未找到学生档案信息" << std::endl;
        return 1;
    }

    std::cout << "当前信息：" << my_info.toString() << std::endl;

    std::cout << "\n===== 修改个人信息 =====" << std::endl;
    StudentModel new_info = my_info;
    new_info.setName("张三三");
    new_info.setMajor("人工智能");
    new_info.setGrade("2024级");
    new_info.setPhone("13900139000");

    // 执行更新
    auto update_res = student_service.updateStudentInfo(login_user, new_info);
    printStudentOpResult(update_res);

    std::cout << "\n===== 验证修改结果 =====" << std::endl;
    auto updated_info = student_service.getStudentInfo(login_user);
    std::cout << "更新后：" << updated_info.toString() << std::endl;

    std::cout << "\n===== 查询个人成绩 =====" << std::endl;
    auto my_scores = student_service.getMyScore(login_user);

    return 0;
}