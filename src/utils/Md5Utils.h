#ifndef MD5_UTILS_H
#define MD5_UTILS_H

#include <string>

// MD5 加密工具类（密码加密用）
class MD5Utils
{
public:
    // 静态方法：对字符串进行 MD5 加密（32位小写）
    static std::string encrypt(const std::string &input);
};

#endif // MD5_UTILS_H