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

typedef struct {
  uint8_t lane;                /* 0: Left, 1: Middle, 2: Right */
  uint8_t side;                /* 0: Left side, 1: Right side of the bamboo */
  int32_t y;                   /* Vertical position */
  uint32_t survival_time_ticks; /* Counts periodic timer ticks */
} PJ_PANDA_t;

extern PJ_PANDA_t panda;
#endif // __PJ_PANDA_GAME_H__
