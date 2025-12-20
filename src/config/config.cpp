#include "Config.h"
#include <fstream>
#include <sstream>

Config *Config::instance = nullptr;

Config *Config::getInstance()
{
    if (instance == nullptr)
    {
        instance = new Config();
    }
    return instance;
}

Config::Config()
{
    loadIni("conf/server.ini");
}

void Config::loadIni(const string &ini_path)
{
    ifstream file(ini_path);
    if (!file.is_open())
    {
        server_conf.port = 8080;
        server_conf.buffer_size = 4096;
        server_conf.backlog = 5;
        path_conf.view_dir = "res/view/";
        path_conf.public_dir = "res/public/";
        path_conf.db_path = "bin/score_db.db";
        return;
    }

    string line;
    string current_section;
    while (getline(file, line))
    {
        // 跳过空行和注释行
        if (line.empty() || line[0] == ';')
            continue;
        if (line[0] == '[' && line.back() == ']')
        {
            current_section = line.substr(1, line.size() - 2);
            continue;
        }

        size_t eq_pos = line.find('=');
        if (eq_pos == string ::npos)
            continue;

        string key = line.substr(0, eq_pos);
        string value = line.substr(eq_pos + 1);

        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);

        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        size_t comment_pos = value.find(';');
        if (comment_pos != string::npos)
            value = value.substr(0, comment_pos);
        value.erase(value.find_last_not_of(" \t") + 1);

        if (current_section == "server")
        {
            if (key == "port")
                server_conf.port = stoi(value);
            else if (key == "buffer_size")
                server_conf.buffer_size = stoi(value);
            else if (key == "backlog")
                server_conf.backlog = stoi(value);
        }
        else if (current_section == "path")
        {
            if (key == "view_dir")
                path_conf.view_dir = value;
            else if (key == "public_dir")
                path_conf.public_dir = value;
            else if (key == "db_path")
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