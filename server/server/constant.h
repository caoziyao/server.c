//
//  constant.h
//  server
//
//  Created by working on 2018/3/13.
//  Copyright © 2018年 working. All rights reserved.
//

#ifndef constant_h
#define constant_h

// EV_SET(kev, ident, filter, flags, fflags, data, udata);
typedef enum _GwKQueueFilter{
    GwKQueueFilterRead = 0x01,
    GwKQueueFilterWrite = 0x02
} GwKQueueFilter;


// EV_SET(kev, ident, filter, flags, fflags, data, udata);
typedef enum _GwKQueueFlag{
    GwKQueueFlagAdd = 0x01,
    GwKQueueFlagEnable = 0x02,
    GwKQueueFlagDelete  = 0x04
} GwKQueueFlag;


#endif /* constant_h */
