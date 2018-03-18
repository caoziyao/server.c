//
//  gwstring.h
//  server
//
//  Created by working on 2018/3/19.
//  Copyright © 2018年 working. All rights reserved.
//

#ifndef gwstring_h
#define gwstring_h

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct _GwString GwString;

GwString *
GwStringStr(char *str);

#endif /* gwstring_h */
