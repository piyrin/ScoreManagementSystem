cmake_minimum_required(VERSION 3.10)
project(ScoreManagementSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    src/config
    src/core/Controller
    src/core/Model
    src/dao
    src/network
    src/service
    src/utils
    lib/sqlite
)

# Source files
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "main.cpp"
    "lib/sqlite/sqlite3.c"
)

# Executable
add_executable(webServer ${SOURCES})

# Link libraries (if needed, for Windows sometimes ws2_32 is needed for networking)
if(WIN32)
    target_link_libraries(webServer ws2_32)
endif()
