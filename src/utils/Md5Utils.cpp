#include "MD5Utils.h"
#include <openssl/md5.h>
#include <iomanip>
#include <sstream>

// MD5 加密实现（依赖 OpenSSL 库，编译时需链接 -lcrypto）
std::string MD5Utils::encrypt(const std::string &input)
{
    unsigned char md5_buf[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char *>(input.c_str()), input.size(), md5_buf);

    // 转换为 32 位小写字符串
    std::stringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(md5_buf[i]);
    }
    return ss.str();
}