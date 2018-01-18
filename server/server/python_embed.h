//
//  python_embed.h
//  server
//
//  Created by working on 2018/1/17.
//  Copyright © 2018年 working. All rights reserved.
//

#ifndef python_embed_h
#define python_embed_h

#include <stdio.h>
#include <Python.h>

void
initPythonEnv();

char *
callPythonFun(char *request);


#endif /* python_embed_h */
