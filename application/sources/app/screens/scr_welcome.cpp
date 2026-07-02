#include "scr_welcome.h"
#include "scr_setting.h"
#include <stdlib.h>

static int16_t bug_y_1;
static int16_t bug_y_2;
static int16_t arrow_x;
static int16_t arrow_y;
static int16_t panda_y;
static uint32_t tick_count = 0;

static void view_scr_welcome();

view_dynamic_t dyn_view_welcome = {{
                                       .item_type = ITEM_TYPE_DYNAMIC,
                                   },
                                   view_scr_welcome};

view_screen_t scr_welcome = {
    &dyn_view_welcome,
    ITEM_NULL,
    ITEM_NULL,

    .focus_item = 0,
};

void view_scr_welcome()
{
  view_render.clear();

  view_render.drawBitmap(16, 0, bamboo, 8, 64, WHITE);
  view_render.drawBitmap(104, 0, bamboo, 8, 64, WHITE);

  // Draw background dynamic particles (simulating wind/leaves blowing
  // leftwards)
  view_render.drawPixel((128 - (tick_count * 2)) % 138 - 10, 15, WHITE);
  view_render.drawPixel((128 - (tick_count * 3)) % 138 - 10, 35, WHITE);
  view_render.drawPixel((128 - (tick_count * 1)) % 138 - 10, 48, WHITE);

  // Draw crawling bug on the left bamboo
  view_render.drawBitmap(8, bug_y_2, bug_left_down, 8, 10, WHITE);
  view_render.drawBitmap(24, bug_y_1, bug_right, 8, 10, WHITE);

  // Draw panda on the right bamboo (facing left/right)
  view_render.drawBitmap(92, panda_y, panda_left, 16, 16, WHITE);

  // Draw the shooting arrow
  view_render.drawBitmap(arrow_x, arrow_y, arrow, 10, 5, WHITE);

  // Draw "PANDA JUMP" Title in the center (Size 2)
  view_render.setTextSize(2);
  view_render.setTextColor(WHITE);
  view_render.setCursor(34, 10);
  view_render.print("PANDA");
  view_render.setCursor(40, 28);
  view_render.print("JUMP");

  // Draw flashing instruction "PRESS MODE TO PLAY"
  if ((tick_count / 4) % 2 == 0)
  {
    view_render.setTextSize(1);
    view_render.setCursor(10, 52);
    view_render.print("PRESS MODE TO PLAY");
  }
}

void view_scr_welcome_update() {}

void scr_welcome_handle(ak_msg_t *msg)
{
  switch (msg->sig)
  {
  case SCREEN_ENTRY:
  {
    APP_DBG_SIG("SCREEN_ENTRY\n");
    bug_y_1 = 64;
    bug_y_2 = 0;
    arrow_x = -15;
    arrow_y = 15 + (rand() % 20);
    panda_y = 64;
    tick_count = 0;

    if (game_settings.sound_en)
    {
      BUZZER_PlaySound(BUZZER_SOUND_PANDA_THEME);
    }
    timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK,
              AC_DISPLAY_WELCOME_TEXT_ANIM_TICK_INTERVAL, TIMER_PERIODIC);
    timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE,
              AC_DISPLAY_IDLE_INTERVAL, TIMER_ONE_SHOT);
  }
  break;

  case AC_DISPLAY_WELCOME_TEXT_ANIM_TICK:
  {
    APP_DBG_SIG("AC_DISPLAY_WELCOME_TEXT_ANIM_TICK\n");

    tick_count++;

    // Bug crawls up
    bug_y_1 -= 1;
    if (bug_y_1 < -10)
    {
      bug_y_1 = 64;
    }

    // Bug crawls down
    bug_y_2 += 1;
    if (bug_y_2 > 64)
    {
      bug_y_2 = 0;
    }
    // Arrow shoots across
    arrow_x += 4;
    if (arrow_x > 128)
    {
      arrow_x = -15;
      arrow_y = 15 + (rand() % 20);
    }

    // Panda crawls up
    panda_y -= 1;
    if (panda_y < -10)
    {
      panda_y = 64;
    }
  }
  break;

  case AC_DISPLAY_BUTON_MODE_PRESSED:
  {
    APP_DBG_SIG("AC_DISPLAY_BUTON_MODE_PRESSED\n");
    timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK);
    SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
  }
  break;

  case AC_DISPLAY_SHOW_IDLE:
  {
    APP_DBG_SIG("AC_DISPLAY_SHOW_IDLE\n");
    // timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK);
    // SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
  }
  break;

  case AC_DISPLAY_BUTON_UP_PRESSED:
  case AC_DISPLAY_BUTON_DOWN_PRESSED:
  {
    APP_DBG_SIG("AC_DISPLAY_BUTON_%s_PRESSED\n",
                msg->sig == AC_DISPLAY_BUTON_UP_PRESSED ? "UP" : "DOWN");
    timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK);
    SCREEN_TRAN(scr_qrcode_handle, &scr_qrcode);
  }
  break;

  default:
    break;
  }
}
