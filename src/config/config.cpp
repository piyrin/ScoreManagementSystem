#include"Config.h"
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
    if(!file.is_open())
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
        //识别节
        if(line[0]=='['&&line.back()==']')
        {
            current_section = line.substr(1, line.size() - 2);
            continue;
        }
        //解析键值对
        size_t eq_pos = line.find('=');
        if (eq_pos == string ::npos)
            continue;//没有=跳过
        
        //分割key和value，去掉前后空格
        string key = line.substr(0, eq_pos);
        string value = line.substr(eq_pos + 1);
        //去掉key前后空格
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t")+1);
        //去掉value前后空格
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);
        // 去掉value后面‘;’部分
        size_t comment_pos = value.find(';');
        if(comment_pos!=string::npos)
            value = value.substr(0, comment_pos);
        value.erase(value.find_last_not_of(" \t") + 1);
        // 根据当前节点填到对应结构体
        if (current_section == "server")
        {
            if(key=="port")
                server_conf.port = stoi(value);
            else if(key=="buffer_size")
                server_conf.buffer_size = stoi(value);
            else if(key=="backlog")
                server_conf.backlog = stoi(value);
        }
        else if(current_section=="path")
        {
            if(key=="view_dir")
                path_conf.view_dir = value;
            else if(key=="public_dir")
                path_conf.public_dir = value;
            else if(key=="db_path")
                path_conf.db_path = value;
        }
    }
    file.close();
}

ServerConfig Config::getServerConfig() const
{
    return server_conf;
}

PathConfig Config::getPathConfig() const
{
    return path_conf;
}