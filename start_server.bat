@echo off
chcp 65001
setlocal

set "BIN_DIR=bin"
set "EXE_NAME=score_system.exe"

if not exist "%BIN_DIR%\%EXE_NAME%" (
    echo [错误] 未找到可执行文件: %BIN_DIR%\%EXE_NAME%
    echo [提示] 请先运行 compile.bat 进行编译。
    pause
    exit /b 1
)

echo [INFO] 正在启动服务器...
echo [提示] 程序将在当前目录下查找配置文件 (conf/server.ini) 和资源文件 (res/)。

:: 直接运行，保持当前目录为项目根目录，以便程序能找到 conf/ 和 res/
"%BIN_DIR%\%EXE_NAME%"

if %errorlevel% neq 0 (
    echo [警告] 服务器异常退出，错误码: %errorlevel%
)

endlocal
pause