#ifndef __SCR_SETTING_H__
#define __SCR_SETTING_H__

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"
#include "task_display.h"

/* Struct to store game configuration settings */
typedef struct
{
  uint8_t difficulty; /* 0: EASY, 1: MEDIUM, 2: HARD */
  uint8_t sound_en;   /* 0: OFF, 1: ON */
  uint8_t time_limit; /* 0: 30s, 1: 60s, 2: 90s */
} game_settings_t;

/* Global configuration variable declaration so other screens can access by including this header */
extern game_settings_t game_settings;

/* Setting screen object */
extern view_screen_t scr_setting;

/* Event handler function (FSM) for Setting screen */
extern void scr_setting_handle(ak_msg_t *msg);

#endif //__SCR_SETTING_H__
