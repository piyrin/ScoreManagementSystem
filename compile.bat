@echo off
chcp 65001
set "SRC_DIR=src"
set "BIN_DIR=bin"

if not exist %BIN_DIR% mkdir %BIN_DIR%

echo 正在编译 SQLite...
gcc -c lib/sqlite/sqlite3.c -o lib/sqlite/sqlite3.o

if %errorlevel% neq 0 (
    echo [错误] SQLite 编译失败！
    pause
    exit /b %errorlevel%
)

echo 正在编译项目...
g++ main.cpp %SRC_DIR%/config/*.cpp %SRC_DIR%/utils/*.cpp %SRC_DIR%/network/*.cpp %SRC_DIR%/dao/*.cpp %SRC_DIR%/service/*.cpp %SRC_DIR%/core/Model/*.cpp %SRC_DIR%/core/Controller/*.cpp lib/sqlite/sqlite3.o -o %BIN_DIR%/score_system.exe -Ilib/sqlite -lws2_32 -finput-charset=UTF-8 -fexec-charset=UTF-8

if %errorlevel% equ 0 (
    echo [成功] 编译完成！输出路径：%BIN_DIR%/score_system.exe
    del lib\sqlite\sqlite3.o
) else (
    echo [错误] 编译失败！
    pause
)