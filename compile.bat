@echo off
chcp 65001
set "SRC_DIR=src"
set "BIN_DIR=bin"

if not exist %BIN_DIR% mkdir %BIN_DIR%

g++ %SRC_DIR%/*.cpp %SRC_DIR%/config/*.cpp %SRC_DIR%/utils/*.cpp %SRC_DIR%/network/*.cpp %SRC_DIR%/dao/*.cpp %SRC_DIR%/service/*.cpp %SRC_DIR%/core/Model/*.cpp %SRC_DIR%/core/Controller/*.cpp -o %BIN_DIR%/score_system.exe -lws2_32 -lsqlite3 -finput-charset=UTF-8 -fexec-charset=UTF-8

if %errorlevel% equ 0 (
    echo [成功] 编译完成！输出路径：%BIN_DIR%/score_system.exe
) else (
    echo [错误] 编译失败！
    pause
)