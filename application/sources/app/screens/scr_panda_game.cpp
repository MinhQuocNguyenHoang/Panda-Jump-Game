#include "scr_panda_game.h"

static void view_scr_panda_game();
static uint32_t tick_count = 0;

view_dynamic_t dyn_view_panda_game = {{
                                          .item_type = ITEM_TYPE_DYNAMIC,
                                      },
                                      view_scr_panda_game};

view_screen_t scr_panda_game = {
    &dyn_view_panda_game,
    ITEM_NULL,
    ITEM_NULL,

    .focus_item = 0,
};

static const uint8_t BAMBOO_X[] = {16, 48, 80};

void pj_game_frame_display()
{
  // 1. Draw 3 bamboos
  for (int i = 0; i < 3; i++)
  {
    view_render.drawBitmap(BAMBOO_X[i], 0, bamboo, 8, 64, WHITE);
  }

  // 2. Draw vertical divider between game area and right panel
  view_render.drawFastVLine(93, 0, 64, WHITE);

  // --- SCORE BOX (TOP RIGHT) ---
  // Draw rounded box for score
  view_render.drawRoundRect(96, 2, 30, 27, 3, WHITE);

  // Draw a mini star icon at (107, 6)
  view_render.drawPixel(110, 5, WHITE);
  view_render.drawLine(108, 6, 112, 6, WHITE);
  view_render.drawLine(107, 7, 113, 7, WHITE);
  view_render.drawLine(109, 8, 111, 8, WHITE);
  view_render.drawPixel(108, 9, WHITE);
  view_render.drawPixel(112, 9, WHITE);

  // Print score (centered in the box)
  view_render.setTextSize(1);
  view_render.setTextColor(WHITE);
  view_render.setCursor(101, 16);
  view_render.print("20"); // TODO: replace with your actual score variable when ready

  // --- TIME BOX (BOTTOM RIGHT) ---
  // Draw rounded box for survival time
  view_render.drawRoundRect(96, 33, 30, 28, 3, WHITE);

  // Draw a mini clock icon at center (110, 40)
  view_render.drawCircle(110, 40, 4, WHITE);
  view_render.drawLine(110, 40, 110, 38, WHITE); // Hour hand
  view_render.drawLine(110, 40, 112, 40, WHITE); // Minute hand

  // Calculate and print survival time in seconds
  uint32_t seconds = panda.survival_time_ticks / 2;
  if (seconds < 10) {
    view_render.setCursor(106, 48);
  } else if (seconds < 100) {
    view_render.setCursor(103, 48);
  } else {
    view_render.setCursor(100, 48);
  }
  view_render.print(seconds);
  view_render.print("s");
}

void pj_panda_display()
{
  uint8_t bx = BAMBOO_X[panda.lane];
  uint8_t px;
  if (panda.side == 0)
  {
    px = bx - 12;
    view_render.drawBitmap(px, panda.y, panda_left, 16, 16, WHITE);
  }
  else
  {
    px = bx + 4;
    view_render.drawBitmap(px, panda.y, panda_right, 16, 16, WHITE);
  }
}

static void view_scr_panda_game()
{
  // Draw falling/blowing leaves (leaf effect)
  view_render.drawPixel((128 - (tick_count * 2)) % 138 - 10, 15, WHITE);
  view_render.drawPixel((128 - (tick_count * 3)) % 138 - 10, 35, WHITE);
  view_render.drawPixel((128 - (tick_count * 1)) % 138 - 10, 48, WHITE);

  pj_game_frame_display();
  pj_panda_display();
}

void scr_panda_game_handle(ak_msg_t *msg)
{
  switch (msg->sig)
  {
  case SCREEN_ENTRY:
  {
    APP_DBG_SIG("PANDA SCREEN_ENTRY\n");
    if (game_settings.sound_en)
    {
      BUZZER_PlaySound(BUZZER_SOUND_LETS_GO);
    }
    BUZZER_PlaySound(BUZZER_SOUND_LETS_GO);
    tick_count = 0;
    timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);
    task_post_pure_msg(PJ_PANDA_GAME_ID, PJ_PANDA_GAME_SETUP);
    task_post_pure_msg(PJ_BUG_GAME_ID, PJ_BUG_GAME_SETUP);
    timer_set(PJ_PANDA_GAME_ID, PJ_GAME_TIME_TICK, PJ_GAME_TIME_TICK_INTERVAL,
              TIMER_PERIODIC);
    timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_PANDA_GAME_UPDATE, 100,
              TIMER_PERIODIC);
  }
  break;

  case PJ_GAME_TIME_TICK:
  {
    task_post_pure_msg(PJ_BUG_GAME_ID, PJ_BUG_GAME_UPDATE);
  }
  break;

  case AC_DISPLAY_PANDA_GAME_UPDATE:
  {
    tick_count++;
  }
  break;

  case AC_DISPLAY_BUTON_UP_PRESSED:
  {
    APP_DBG_SIG("PANDA LEFT\n");
    task_post_pure_msg(PJ_PANDA_GAME_ID, PJ_PANDA_JUMP_RIGHT);
  }
  break;

  case AC_DISPLAY_BUTON_DOWN_PRESSED:
  {
    APP_DBG_SIG("PANDA RIGHT\n");
    task_post_pure_msg(PJ_PANDA_GAME_ID, PJ_PANDA_JUMP_LEFT);
  }
  break;

  case AC_DISPLAY_BUTON_MODE_RELEASED:
  {
    APP_DBG_SIG("PANDA UP\n");
    task_post_pure_msg(PJ_PANDA_GAME_ID, PJ_PANDA_GAME_UP);
  }
  break;

  case AC_DISPLAY_BUTON_MODE_LONG_RELEASED:
  {
    APP_DBG_SIG("PANDA DOWN\n");
    task_post_pure_msg(PJ_PANDA_GAME_ID, PJ_PANDA_GAME_DOWN);
  }
  break;

  default:
    break;
  }
}
