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

# Find OpenSSL
if(MINGW)
    if(NOT DEFINED OPENSSL_ROOT_DIR AND DEFINED ENV{OPENSSL_ROOT_DIR})
        set(OPENSSL_ROOT_DIR "$ENV{OPENSSL_ROOT_DIR}")
    endif()

    if(DEFINED OPENSSL_ROOT_DIR)
        set(_OPENSSL_ROOT "${OPENSSL_ROOT_DIR}")
        set(_OPENSSL_INCLUDE "${_OPENSSL_ROOT}/include")
        set(_OPENSSL_DLL_DIR "${_OPENSSL_ROOT}/bin")
        set(_OPENSSL_DEF_DIR "${_OPENSSL_ROOT}/lib/VC/x64/MD")

        set(_CRYPTO_DEF "${_OPENSSL_DEF_DIR}/libcrypto.def")
        set(_SSL_DEF "${_OPENSSL_DEF_DIR}/libssl.def")
        set(_CRYPTO_DLL "${_OPENSSL_DLL_DIR}/libcrypto-3-x64.dll")
        set(_SSL_DLL "${_OPENSSL_DLL_DIR}/libssl-3-x64.dll")

        if(EXISTS "${_OPENSSL_INCLUDE}/openssl/md5.h" AND EXISTS "${_CRYPTO_DEF}" AND EXISTS "${_SSL_DEF}" AND EXISTS "${_CRYPTO_DLL}" AND EXISTS "${_SSL_DLL}")
            if(NOT CMAKE_DLLTOOL)
                set(CMAKE_DLLTOOL dlltool)
            endif()

            set(_OPENSSL_MINGW_IMPORT_DIR "${CMAKE_BINARY_DIR}/openssl-mingw")
            file(MAKE_DIRECTORY "${_OPENSSL_MINGW_IMPORT_DIR}")

            set(_CRYPTO_A "${_OPENSSL_MINGW_IMPORT_DIR}/libcrypto.dll.a")
            set(_SSL_A "${_OPENSSL_MINGW_IMPORT_DIR}/libssl.dll.a")

            if(NOT EXISTS "${_CRYPTO_A}")
                execute_process(
                    COMMAND "${CMAKE_DLLTOOL}" -D "${_CRYPTO_DLL}" -d "${_CRYPTO_DEF}" -l "${_CRYPTO_A}"
                    RESULT_VARIABLE _CRYPTO_DLLTOOL_RET
                    ERROR_VARIABLE _CRYPTO_DLLTOOL_ERR
                )
                if(NOT _CRYPTO_DLLTOOL_RET EQUAL 0)
                    message(WARNING "Failed to generate MinGW import lib for OpenSSL crypto: ${_CRYPTO_DLLTOOL_ERR}")
                endif()
            endif()

            if(NOT EXISTS "${_SSL_A}")
                execute_process(
                    COMMAND "${CMAKE_DLLTOOL}" -D "${_SSL_DLL}" -d "${_SSL_DEF}" -l "${_SSL_A}"
                    RESULT_VARIABLE _SSL_DLLTOOL_RET
                    ERROR_VARIABLE _SSL_DLLTOOL_ERR
                )
                if(NOT _SSL_DLLTOOL_RET EQUAL 0)
                    message(WARNING "Failed to generate MinGW import lib for OpenSSL ssl: ${_SSL_DLLTOOL_ERR}")
                endif()
            endif()

            set(OPENSSL_INCLUDE_DIR "${_OPENSSL_INCLUDE}" CACHE PATH "" FORCE)
            set(OPENSSL_CRYPTO_LIBRARY "${_CRYPTO_A}" CACHE FILEPATH "" FORCE)
            set(OPENSSL_SSL_LIBRARY "${_SSL_A}" CACHE FILEPATH "" FORCE)

            if(EXISTS "${_CRYPTO_A}" AND EXISTS "${_SSL_A}")
                if(NOT TARGET OpenSSL::Crypto)
                    add_library(OpenSSL::Crypto SHARED IMPORTED)
                    set_target_properties(OpenSSL::Crypto PROPERTIES
                        IMPORTED_IMPLIB "${_CRYPTO_A}"
                        IMPORTED_LOCATION "${_CRYPTO_DLL}"
                        INTERFACE_INCLUDE_DIRECTORIES "${_OPENSSL_INCLUDE}"
                    )
                endif()

                if(NOT TARGET OpenSSL::SSL)
                    add_library(OpenSSL::SSL SHARED IMPORTED)
                    set_target_properties(OpenSSL::SSL PROPERTIES
                        IMPORTED_IMPLIB "${_SSL_A}"
                        IMPORTED_LOCATION "${_SSL_DLL}"
                        INTERFACE_INCLUDE_DIRECTORIES "${_OPENSSL_INCLUDE}"
                    )
                endif()

                set(OpenSSL_FOUND TRUE)
            endif()
        endif()
    endif()
endif()

if(NOT TARGET OpenSSL::Crypto)
    find_package(OpenSSL REQUIRED)
endif()

if(OpenSSL_FOUND AND OPENSSL_INCLUDE_DIR)
    include_directories(${OPENSSL_INCLUDE_DIR})
endif()

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
    target_link_libraries(webServer ws2_32 OpenSSL::Crypto)
else()
    target_link_libraries(webServer OpenSSL::Crypto dl pthread)
endif()
