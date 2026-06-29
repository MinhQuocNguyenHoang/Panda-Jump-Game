#ifndef __PJ_PANDA_GAME_H__
#define __PJ_PANDA_GAME_H__

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

#define STEP_PANDA_AXIS_Y (1)
#define SIZE_BITMAP_AXIS_Y (16)
#define SIZE_BITMAP_AXIS_X (16)
#define AXIS_Y_PANDA (32)
#define AXIS_X_PANDA (0)

typedef struct {
  uint32_t x, y;

} PJ_PANDA_t;

extern PJ_PANDA_t panda;
#endif // __PJ_PANDA_GAME_H__
