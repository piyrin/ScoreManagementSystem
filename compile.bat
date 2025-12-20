@echo off
chcp 65001
setlocal

echo [INFO] 检查编译环境...

where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo [错误] 未找到 cmake 命令。请安装 CMake 并将其添加到系统 PATH 中。
    echo [提示] 下载地址: https://cmake.org/download/
    pause
    exit /b 1
)

where g++ >nul 2>nul
if %errorlevel% neq 0 (
    echo [错误] 未找到 g++ 命令。请安装 MinGW-w64 并将其添加到系统 PATH 中。
    echo [提示] 下载地址: https://www.mingw-w64.org/
    pause
    exit /b 1
)

if not exist build mkdir build
cd build

echo [INFO] 正在配置项目 (CMake)...
cmake -G "MinGW Makefiles" ..
if %errorlevel% neq 0 (
    echo [错误] CMake 配置失败。请检查 CMakeLists.txt 或环境配置。
    echo [提示] 如果是 OpenSSL 相关错误，请确保已安装 OpenSSL 开发库并设置 OPENSSL_ROOT_DIR 环境变量。
    cd ..
    pause
    exit /b 1
)

echo [INFO] 正在编译项目...
cmake --build .
if %errorlevel% neq 0 (
    echo [错误] 编译失败。
    cd ..
    pause
    exit /b 1
)

cd ..
if not exist bin mkdir bin

echo [INFO] 正在安装/复制可执行文件...
if exist build\webServer.exe (
    copy /Y build\webServer.exe bin\score_system.exe >nul
    echo [成功] 编译完成！可执行文件已生成: bin\score_system.exe
) else (
    echo [错误] 未找到生成的 webServer.exe，请检查 build 目录。
    pause
    exit /b 1
)

endlocal
pause