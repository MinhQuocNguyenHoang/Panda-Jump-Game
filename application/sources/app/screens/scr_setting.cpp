#include "scr_setting.h"
#include "screens.h"
#include "app_dbg.h"
#include "buzzer.h"

/* Định nghĩa và khởi tạo biến cấu hình toàn cục */
game_settings_t game_settings = {
    .difficulty = 1, /* Mặc định độ khó là MEDIUM (1) */
    .sound_en = 0    /* Mặc định bật âm thanh (1) */
};

/* Vị trí dòng được chọn hiện hành: 0 = Difficulty, 1 = Sound, 2 = Back */
static uint8_t select_index = 0;

static void view_scr_setting();

view_dynamic_t dyn_view_setting = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_setting};

view_screen_t scr_setting = {
    &dyn_view_setting,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

/* Hàm vẽ nội dung màn hình Cài đặt lên buffer hiển thị OLED */
static void view_scr_setting()
{
  view_render.clear();

  // Vẽ tiêu đề màn hình
  view_render.setTextSize(1);
  view_render.setTextColor(WHITE);
  view_render.setCursor(32, 2);
  view_render.print("--- SETTINGS ---");

  // Vẽ dòng 1: Cấu hình độ khó (DIFFICULTY)
  view_render.setCursor(15, 18);
  view_render.print("DIFFICULTY: ");
  if (game_settings.difficulty == 0)
  {
    view_render.print("EASY");
  }
  else if (game_settings.difficulty == 1)
  {
    view_render.print("MEDIUM");
  }
  else
  {
    view_render.print("HARD");
  }

  // Vẽ dòng 2: Cấu hình âm thanh (SOUND)
  view_render.setCursor(15, 33);
  view_render.print("SOUND:      ");
  if (game_settings.sound_en == 1)
  {
    view_render.print("ON");
  }
  else
  {
    view_render.print("OFF");
  }

  // Vẽ dòng 3: Quay về Menu
  view_render.setCursor(15, 48);
  view_render.print("BACK TO MENU");

  // Vẽ con trỏ lựa chọn hình chữ ">"
  int16_t pointer_y = 18 + (select_index * 15);
  view_render.setCursor(5, pointer_y);
  view_render.print(">");
}

/* Hàm xử lý sự kiện (FSM) cho màn hình Cài đặt */
void scr_setting_handle(ak_msg_t *msg)
{
  switch (msg->sig)
  {
  case SCREEN_ENTRY:
  {
    APP_DBG("[scr_setting] SCREEN_ENTRY - Reset index\n");
    select_index = 0; // Bắt đầu từ dòng đầu tiên
    timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE,
              AC_DISPLAY_IDLE_INTERVAL, TIMER_ONE_SHOT);
  }
  break;

  case AC_DISPLAY_SHOW_IDLE:
  {
    APP_DBG("[scr_setting] Timeout - Quay về màn hình chờ Idle\n");
    timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);
    SCREEN_TRAN(scr_idle_handle, &scr_idle);
  }
  break;

  case AC_DISPLAY_BUTON_UP_PRESSED:
  {
    APP_DBG("[scr_setting] Nút UP nhấn\n");
    timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE,
              AC_DISPLAY_IDLE_INTERVAL, TIMER_ONE_SHOT);

    if (select_index > 0)
    {
      select_index--;
    }
    else
    {
      select_index = 2; // Xoay vòng về dòng cuối
    }
    if (game_settings.sound_en)
    {
      BUZZER_PlaySound(BUZZER_SOUND_CLICK);
    }
  }
  break;

  case AC_DISPLAY_BUTON_DOWN_PRESSED:
  {
    APP_DBG("[scr_setting] Nút DOWN nhấn\n");
    timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE,
              AC_DISPLAY_IDLE_INTERVAL, TIMER_ONE_SHOT);

    if (select_index < 2)
    {
      select_index++;
    }
    else
    {
      select_index = 0; // Xoay vòng về dòng đầu
    }

    if (game_settings.sound_en)
    {
      BUZZER_PlaySound(BUZZER_SOUND_CLICK);
    }
  }
  break;

  case AC_DISPLAY_BUTON_MODE_PRESSED:
  {
    APP_DBG("[scr_setting] Nút MODE nhấn để tương tác\n");
    timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE,
              AC_DISPLAY_IDLE_INTERVAL, TIMER_ONE_SHOT);

    switch (select_index)
    {
    case 0:
    { // Đổi Độ khó
      game_settings.difficulty = (game_settings.difficulty + 1) % 3;
      APP_DBG("[scr_setting] Difficulty set: %d\n", game_settings.difficulty);
      if (game_settings.sound_en)
      {
        BUZZER_PlaySound(BUZZER_SOUND_CLICK);
      }
    }
    break;

    case 1:
    { // Đổi Âm thanh
      game_settings.sound_en = !game_settings.sound_en;
      APP_DBG("[scr_setting] Sound set: %d\n", game_settings.sound_en);
      if (game_settings.sound_en)
      {
        BUZZER_PlaySound(BUZZER_SOUND_CLICK);
      }
    }
    break;

    case 2:
    { // Thoát về Menu
      APP_DBG("[scr_setting] Chọn BACK - Thoát ra Menu chính\n");
      timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);
      if (game_settings.sound_en)
      {
        BUZZER_PlaySound(BUZZER_SOUND_CLICK);
      }
      SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
    }
    break;

    default:
      break;
    }
  }
  break;

  default:
    break;
  }
}
