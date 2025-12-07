#include <sqlite3.h>
#include <iostream>
#include "SqliteUtils.h"
#include<string>
#include<../config/Config.h"

//静态成员变量初始化：单例指针
SqlitUtils* SqlitUtils::instance = nullptr;