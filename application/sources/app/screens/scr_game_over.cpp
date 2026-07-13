#include "scr_game_over.h"
#include "scr_panda_game.h"
#include "pj_panda_game.h"
#include "buzzer.h"

static void view_scr_game_over();

view_dynamic_t dyn_view_game_over = {{
                                         .item_type = ITEM_TYPE_DYNAMIC,
                                     },
                                     view_scr_game_over};

view_screen_t scr_game_over = {
    &dyn_view_game_over,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

static void drawScoreStar(int16_t x, int16_t y)
{
  view_render.drawPixel(x + 3, y, WHITE);
  view_render.drawLine(x + 1, y + 1, x + 5, y + 1, WHITE);
  view_render.drawLine(x, y + 2, x + 6, y + 2, WHITE);
  view_render.drawLine(x + 2, y + 3, x + 4, y + 3, WHITE);
  view_render.drawPixel(x + 1, y + 4, WHITE);
  view_render.drawPixel(x + 5, y + 4, WHITE);
}

static void drawClockIcon(int16_t x, int16_t y)
{
  view_render.drawCircle(x + 4, y + 4, 4, WHITE);
  view_render.drawLine(x + 4, y + 4, x + 4, y + 2, WHITE); // Hour hand
  view_render.drawLine(x + 4, y + 4, x + 6, y + 4, WHITE); // Minute hand
}
static void view_scr_game_over()
{
  view_render.clear();
  view_render.drawRoundRect(2, 4, 124, 56, 4, WHITE);
  view_render.fillRect(4, 6, 120, 11, WHITE);
  view_render.setTextSize(1);
  view_render.setTextColor(BLACK);
  view_render.setCursor(37, 8);
  view_render.print("GAME OVER");
  view_render.setTextColor(WHITE);
  view_render.drawFastVLine(64, 20, 22, WHITE);

  uint8_t score_x_icon = (panda.score < 10) ? 24 : (panda.score < 100) ? 21
                                                                       : 18;
  uint8_t score_x_text = score_x_icon + 11;
  drawScoreStar(score_x_icon, 24);
  view_render.setCursor(score_x_text, 23);
  view_render.print(panda.score);

  // --- Right column: Real survival time ---
  uint32_t seconds = panda.survival_time_ticks / 2;
  uint8_t time_x_icon = (seconds < 10) ? 83 : (seconds < 100) ? 80
                                                              : 77;
  uint8_t time_x_text = time_x_icon + 12;
  drawClockIcon(time_x_icon, 23);
  view_render.setCursor(time_x_text, 23);
  view_render.print(seconds);
  view_render.print("s");

  view_render.drawFastHLine(2, 45, 124, WHITE);
  view_render.setCursor(7, 49);
  view_render.print("UP:Retry  MODE:Exit");
}

void scr_game_over_handle(ak_msg_t *msg)
{
  switch (msg->sig)
  {
  case SCREEN_ENTRY:
  {
    APP_DBG_SIG("SCR_GAME_OVER_ENTRY\n");
    if (game_settings.sound_en)
    {
      BUZZER_PlaySound(BUZZER_SOUND_BANG);
    }
  }
  break;

  case AC_DISPLAY_BUTON_UP_RELEASED:
  case AC_DISPLAY_BUTON_UP_PRESSED:
  {
    SCREEN_TRAN(scr_panda_game_handle, &scr_panda_game);
  }
  break;

  case AC_DISPLAY_BUTON_MODE_RELEASED:
  case AC_DISPLAY_BUTON_MODE_PRESSED:
  {
    SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
  }
  break;

  default:
    break;
  }
}
