//
//  gw_conf_file.c
//  server
//
//  Created by working on 2018/4/2.
//  Copyright © 2018年 working. All rights reserved.
//  配置解析模块，配置文件采用 lua 脚本

#include "gw_conf_file.h"


void
GwConfRead() {
    GuwLuaExeclTable("server_lua/config.lua");
}
