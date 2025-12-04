#include"config.h"
#include<fstream>
#include<sstream>
//初始化静态指针
Config *Config::instance = nullptr;

Config* Config::getInstance()
{
    if(instance==nullptr)
    {
        instance = new Config();
    }
    return instance;
}

Config::Config()
{
    //加载默认路径的配置文件
    loadIni("conf/server.ini");
}

void Config::loadIni(const string &ini_path)
{
    //打开配置文件
    ifstream file(ini_path);
    //打开失败，使用默认配置
    if(!file.is_open)
    {
        server_conf.port = 8080;
        server_conf.buffer_size = 4096;
        server_conf.backlog = 5;
        path_conf.view_dir = "res/view/";
        path_conf.public_dir = "res/public/";
        path_conf.db_path = "bin/score_db.db";
        return;
    }
    //逐行读取
    string line;
    string current_section;//记录当前处于哪个节
    while(getline(file,line))
    {
        //跳过空行和注释行
        if (line.empty() || line[0] == ';')
            continue;
        if()
    }

}

ServerConfig Config::getServerConfig() const
{
    return server_conf;
}

PathConfig Config::getPathConfig() const
{
    return path_conf;
}