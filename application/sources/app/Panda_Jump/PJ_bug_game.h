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

typedef struct
{
    int16_t x;
    int16_t y;
    int16_t speed;
    bool direction; // true = right, false = left
} bug_t;

extern bug_t bug;
#endif /*__BUG_H__*/