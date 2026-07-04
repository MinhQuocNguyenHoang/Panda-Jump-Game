#include "scr_setting.h"
#include "screens.h"
#include "app_dbg.h"
#include "buzzer.h"

/* Global configuration variable definition and initialization */
game_settings_t game_settings = {
    .difficulty = 1, /* Default difficulty is MEDIUM (1) */
    .sound_en = 0,   /* Default sound is OFF (0) */
    .time_limit = 1  /* Default time limit is 60s (1) */
};

/* Position of current selected index: 0 = Difficulty, 1 = Sound, 2 = Time Limit, 3 = Back */
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

/* Render setting screen content onto the OLED display buffer */
// General function to draw a setting item (supports label, value, and selection pointer)
static void drawSettingItem(int16_t y, const char *label, const char *value, bool is_selected)
{
  view_render.setCursor(15, y);
  view_render.print(label);
  if (value != nullptr)
  {
    view_render.print(value);
  }

  // Draw selection pointer if this item is selected
  if (is_selected)
  {
    view_render.setCursor(5, y);
    view_render.print(">");
  }
}

static void view_scr_setting()
{
  view_render.clear();

  // Draw screen title
  view_render.setTextSize(1);
  view_render.setTextColor(WHITE);
  view_render.setCursor(15, 2);
  view_render.print("--- SETTINGS ---");

  // Row 1: DIFFICULTY at y = 16
  const char *diff_str = (game_settings.difficulty == 0) ? "EASY" : (game_settings.difficulty == 1) ? "MEDIUM"
                                                                                                    : "HARD";
  drawSettingItem(16, "DIFFICULTY: ", diff_str, select_index == 0);

  // Row 2: SOUND at y = 27
  const char *sound_str = (game_settings.sound_en == 1) ? "ON" : "OFF";
  drawSettingItem(27, "SOUND:      ", sound_str, select_index == 1);

  // Row 3: TIME LIMIT at y = 38
  const char *time_str = (game_settings.time_limit == 0) ? "30s" : (game_settings.time_limit == 1) ? "60s"
                                                                                                   : "90s";
  drawSettingItem(38, "TIME LIMIT: ", time_str, select_index == 2);

  // Row 4: BACK TO MENU at y = 49
  drawSettingItem(49, "BACK TO MENU", nullptr, select_index == 3);
}

/* Event handler function (FSM) for Setting screen */
void scr_setting_handle(ak_msg_t *msg)
{
  switch (msg->sig)
  {
  case SCREEN_ENTRY:
  {
    APP_DBG("[scr_setting] SCREEN_ENTRY - Reset index\n");
    select_index = 0; // Start from first item
    timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE,
              AC_DISPLAY_IDLE_INTERVAL, TIMER_ONE_SHOT);
  }
  break;

  case AC_DISPLAY_SHOW_IDLE:
  {
    APP_DBG("[scr_setting] Timeout - Return to Idle screen\n");
    timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);
    SCREEN_TRAN(scr_idle_handle, &scr_idle);
  }
  break;

  case AC_DISPLAY_BUTON_UP_PRESSED:
  {
    APP_DBG("[scr_setting] UP button pressed\n");
    timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE,
              AC_DISPLAY_IDLE_INTERVAL, TIMER_ONE_SHOT);

    if (select_index > 0)
    {
      select_index--;
    }
    else
    {
      select_index = 3; // Wrap around to the last item
    }
    if (game_settings.sound_en)
    {
      BUZZER_PlaySound(BUZZER_SOUND_CLICK);
    }
  }
  break;

  case AC_DISPLAY_BUTON_DOWN_PRESSED:
  {
    APP_DBG("[scr_setting] DOWN button pressed\n");
    timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE,
              AC_DISPLAY_IDLE_INTERVAL, TIMER_ONE_SHOT);

    if (select_index < 3)
    {
      select_index++;
    }
    else
    {
      select_index = 0; // Wrap around to the first item
    }

    if (game_settings.sound_en)
    {
      BUZZER_PlaySound(BUZZER_SOUND_CLICK);
    }
  }
  break;

  case AC_DISPLAY_BUTON_MODE_PRESSED:
  {
    APP_DBG("[scr_setting] MODE button pressed to interact\n");
    timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE,
              AC_DISPLAY_IDLE_INTERVAL, TIMER_ONE_SHOT);

    switch (select_index)
    {
    case 0:
    { // Change Difficulty
      game_settings.difficulty = (game_settings.difficulty + 1) % 3;
      APP_DBG("[scr_setting] Difficulty set: %d\n", game_settings.difficulty);
      if (game_settings.sound_en)
      {
        BUZZER_PlaySound(BUZZER_SOUND_CLICK);
      }
    }
    break;

    case 1:
    { // Change Sound
      game_settings.sound_en = !game_settings.sound_en;
      APP_DBG("[scr_setting] Sound set: %d\n", game_settings.sound_en);
      if (game_settings.sound_en)
      {
        BUZZER_PlaySound(BUZZER_SOUND_CLICK);
      }
    }
    break;

    case 2:
    { // Change Time Limit
      game_settings.time_limit = (game_settings.time_limit + 1) % 3;
      APP_DBG("[scr_setting] Time limit set: %d\n", game_settings.time_limit);
      if (game_settings.sound_en)
      {
        BUZZER_PlaySound(BUZZER_SOUND_CLICK);
      }
    }
    break;

    case 3:
    { // Back to Menu
      APP_DBG("[scr_setting] Selected BACK - Exit to main Menu\n");
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
