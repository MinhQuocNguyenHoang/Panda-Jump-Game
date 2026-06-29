#ifndef __SCR_PANDA_GAME_H__
#define __SCR_PANDA_GAME_H__
#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "app.h"
#include "app_dbg.h"
#include "task_display.h"
#include "task_list.h"
#include "view_render.h"

#include "app_eeprom.h"
#include "eeprom.h"

#include "buzzer.h"

#include "PJ_panda_game.h"
#include "screens.h"
#include "screens_bitmap.h"

#define GAME_OFF (0)
#define GAME_PLAY (1)
#define GAME_OVER (2)

extern uint8_t ar_game_state;

extern view_dynamic_t dyn_view_panda_game;
extern view_screen_t scr_panda_game;
extern void scr_panda_game_handle(ak_msg_t *msg);

#endif //__SCR_PANDA_GAME_H__
