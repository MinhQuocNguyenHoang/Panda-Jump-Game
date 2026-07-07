#ifndef __SCR_SETTING_H__
#define __SCR_SETTING_H__

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"
#include "task_display.h"

#include "app_eeprom.h"

/* Global configuration variable declaration so other screens can access by including this header */
extern game_settings_t game_settings;

/* Setting screen object */
extern view_screen_t scr_setting;

/* Event handler function (FSM) for Setting screen */
extern void scr_setting_handle(ak_msg_t *msg);

#endif //__SCR_SETTING_H__
