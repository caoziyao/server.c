//
//  gwupstream.h
//  server
//
//  Created by working on 2018/4/1.
//  Copyright © 2018年 working. All rights reserved.
//

#ifndef gwupstream_h
#define gwupstream_h

#include <stdio.h>
#include "gwpool.h"

typedef struct _GwUpstream GwUpstream;
typedef struct _GwAddress GwAddress;

// 定义 server
struct _GwAddress{
    char            *host;
    unsigned int    port;
};

#endif /* gwupstream_h */
