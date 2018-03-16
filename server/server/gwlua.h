//
//  gwlua.h
//  server
//
//  Created by working on 2018/3/16.
//  Copyright © 2018年 working. All rights reserved.
//

#ifndef gwlua_h
#define gwlua_h

#include <stdio.h>

void
GwLuaInitEnv();

void
GwLuaCloseEnv();

void *
GwLuaDoFile(char *filename);

const char *
GuwLuaExecl(char *filename, char *arg);

#endif /* gwlua_h */
