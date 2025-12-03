@echo off
chcp 65001
set "BIN_DIR=bin"

if not exist %BIN_DIR%/score_system.exe (
    echo [错误] 未找到可执行文件，请先编译！
    pause
    exit /b 1
)

cd %BIN_DIR%
score_system.exe
pause