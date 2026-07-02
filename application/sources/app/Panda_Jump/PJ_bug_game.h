#ifndef __BUG_H__
#define __BUG_H__
#include <stdint.h>
#include <stdio.h>

#include "fsm.h"
#include "message.h"
#include "port.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "task_display.h"
#include "task_list.h"
#include "scr_setting.h"
#include "PJ_panda_game.h"

#define MAX_BUGS (5)

typedef struct
{
    int16_t x;
    int16_t y;
    int16_t speed;
    bool direction; // true = down, false = up
    bool active;    // true = active, false = inactive/dead
    bool side;      // true =  bên phải thân trúc, false = bên trái thân trúc
    uint8_t explosion_ticks; // > 0 if the bug is currently exploding
} bug_t;

extern bug_t bugs[MAX_BUGS];
#endif /*__BUG_H__*/