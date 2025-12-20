#ifndef CONFIG_H
#define CONFIG_H
#include<iostream>
#include<string>
using namespace std;
struct ServerConfig
{
    int port=8080;         //监听端口
    int buffer_size;  //请求缓冲区大小
    int backlog;      //连接等待队列长度
};
struct PathConfig
{
    string view_dir;    //页面文件目录
    string public_dir;  //静态文件目录
    string db_path;     //数据文件库目录
};
class Config
{
    private:
        //存储配置的结构体
        ServerConfig server_conf;
        PathConfig path_conf;

        static Config *instance;

        Config();
        //加载ini配置文件
        void loadIni(const string &ini_path = "conf/server.ini");

    public:
        //避免复制构造和赋值
        Config(const Config&) = delete;
        Config &operator=(const Config&) = delete;

        //获取单例实例
        static Config *getInstance();

        //获取配置
        ServerConfig getServerConfig() const;
        PathConfig getPathConfig() const;
};
#endif 