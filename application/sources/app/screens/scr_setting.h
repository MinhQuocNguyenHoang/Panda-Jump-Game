#ifndef __SCR_SETTING_H__
#define __SCR_SETTING_H__

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"
#include "task_display.h"

/* Cấu trúc lưu trữ cấu hình cài đặt của trò chơi */
typedef struct {
  uint8_t difficulty; /* 0: EASY, 1: MEDIUM, 2: HARD */
  uint8_t sound_en;   /* 0: OFF (Tắt), 1: ON (Bật âm thanh) */
} game_settings_t;

/* Khai báo biến cấu hình toàn cục để các màn hình khác có thể sử dụng bằng cách include file header này */
extern game_settings_t game_settings;

/* Đối tượng màn hình Setting */
extern view_screen_t scr_setting;

/* Hàm xử lý sự kiện của màn hình Setting */
extern void scr_setting_handle(ak_msg_t *msg);

#endif //__SCR_SETTING_H__
