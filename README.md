# 成绩管理系统 (Score Management System)

![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue)
![Build](https://img.shields.io/badge/Build-CMake-green)
![Database](https://img.shields.io/badge/Database-SQLite3-lightgrey)

这是一个基于 C++17 开发的轻量级成绩管理系统，内置了自定义的 HTTP 服务器，通过 Web 界面提供交互。系统设计了三种角色：管理员、教师和学生，分别对应不同的业务功能。

## ✨ 功能介绍

系统实现了以下核心功能：

### 👨‍💼 管理员 (Admin)
拥有系统的最高权限，主要负责基础数据的维护。
- **账号管理**：可以添加**学生**和**教师**的账号，管理系统用户。
- **课程管理**：负责**添加课程**信息，为教学活动做好准备。

### 👩‍🏫 教师 (Teacher)
负责具体的教学评估工作。
- **成绩录入**：为自己教授的课程给学生**打分**（录入成绩）。
- **统计分析**：可以查看课程的**及格率**和**平均分**，掌握班级整体学习情况。

### 👨‍🎓 学生 (Student)
系统的最终服务对象。
- **成绩查询**：登录后可以查看自己**各科的成绩**。
- **绩点查询**：系统会自动计算并显示**绩点 (GPA)**，方便学生了解学业水平。

## 🛠 技术栈

- **编程语言**: C++17
- **构建系统**: CMake
- **数据库**: SQLite3 (轻量级嵌入式数据库)
- **网络核心**: 自研 HTTP Server (基于 Winsock2 Socket 编程)
- **前端界面**: HTML / CSS / JavaScript

## 📂 项目结构

```text
ScoreManagementSystem/
├── bin/                 # 编译输出目录 (可执行文件)
├── build/               # CMake 构建中间文件
├── conf/                # 配置文件 (server.ini)
├── lib/                 # 第三方库 (sqlite)
├── res/                 # 静态资源
│   ├── public/          # CSS, JS, Images
│   └── view/            # HTML 页面模板 (admin, teacher, user)
├── src/                 # 源代码
│   ├── config/          # 配置读取
│   ├── core/            # 核心业务逻辑 (Controller, Model)
│   ├── dao/             # 数据访问层 (Data Access Object)
│   ├── network/         # HTTP 服务器实现
│   ├── service/         # 业务逻辑层
│   └── utils/           # 工具类 (Log, Sqlite)
├── CMakeLists.txt       # CMake 构建脚本
├── compile.bat          # 一键编译脚本
└── start_server.bat     # 启动服务脚本
```

## 🚀 快速开始

### 1. 环境准备
确保您的 Windows 系统已安装：
- C++ 编译器 (推荐 MinGW-w64)
- CMake (3.10 或更高版本)

### 2. 编译项目
我们提供了一键编译脚本，双击运行项目根目录下的 `compile.bat`：
```cmd
compile.bat
```
脚本会自动创建构建目录、运行 CMake 配置并生成可执行文件 `webServer.exe`。

### 3. 运行服务
编译成功后，双击运行 `start_server.bat` 或直接在终端运行：
```cmd
./webServer.exe
```
服务器启动后，请在浏览器中访问控制台输出的地址（通常是 `http://localhost:8080`）。

## 📝 数据库说明
项目使用 SQLite 数据库，数据库文件通常会在第一次运行时自动初始化。
- 用户表 (user)
- 学生表 (student)
- 教师表 (teacher)
- 课程表 (course)
- 成绩表 (score)
