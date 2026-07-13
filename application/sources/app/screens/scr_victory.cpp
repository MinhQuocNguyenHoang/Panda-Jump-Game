#include "scr_victory.h"
#include "scr_panda_game.h"
#include "pj_panda_game.h"
#include "buzzer.h"

static void view_scr_victory();

view_dynamic_t dyn_view_victory = {{
                                       .item_type = ITEM_TYPE_DYNAMIC,
                                   },
                                   view_scr_victory};

view_screen_t scr_victory = {
    &dyn_view_victory,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

static void drawSparkleStar(int16_t x, int16_t y)
{
  view_render.drawPixel(x + 2, y, WHITE);
  view_render.drawLine(x + 1, y + 1, x + 3, y + 1, WHITE);
  view_render.drawLine(x, y + 2, x + 4, y + 2, WHITE);
  view_render.drawLine(x + 1, y + 3, x + 3, y + 3, WHITE);
  view_render.drawPixel(x + 2, y + 4, WHITE);
}

static void drawVectorTrophy(int16_t x, int16_t y)
{
  // Bowl
  view_render.drawTriangle(x + 2, y, x + 18, y, x + 10, y + 12, WHITE);
  view_render.fillTriangle(x + 4, y + 1, x + 16, y + 1, x + 10, y + 10, WHITE);

  // Handles
  view_render.drawCircle(x + 2, y + 4, 3, WHITE);
  view_render.drawCircle(x + 18, y + 4, 3, WHITE);

  // Stem
  view_render.drawFastVLine(x + 10, y + 10, 6, WHITE);
  view_render.drawFastVLine(x + 9, y + 10, 6, WHITE);
  view_render.drawFastVLine(x + 11, y + 10, 6, WHITE);

  // Base
  view_render.fillRect(x + 5, y + 16, 11, 4, WHITE);
}
static void view_scr_victory()
{
  view_render.clear();
  view_render.drawRoundRect(2, 4, 124, 56, 4, WHITE);
  view_render.fillRect(4, 6, 120, 11, WHITE);
  view_render.setTextSize(1);
  view_render.setTextColor(BLACK);
  view_render.setCursor(40, 8);
  view_render.print("VICTORY!");
  view_render.setTextColor(WHITE);
  view_render.drawFastVLine(52, 20, 22, WHITE);

  drawVectorTrophy(16, 23);
  drawSparkleStar(10, 28);
  drawSparkleStar(40, 28);

  view_render.setCursor(58, 22);
  view_render.print("YOU WIN!");
  view_render.setCursor(58, 33);
  view_render.print("Score: ");
  view_render.print(panda.score);

  view_render.drawFastHLine(2, 45, 124, WHITE);
  view_render.setCursor(7, 49);
  view_render.print("UP:Play  MODE:Exit");
}

void scr_victory_handle(ak_msg_t *msg)
{
  switch (msg->sig)
  {
  case SCREEN_ENTRY:
  {
    APP_DBG_SIG("SCR_VICTORY_ENTRY\n");
    if (game_settings.sound_en)
    {
      BUZZER_PlaySound(BUZZER_SOUND_MERRY_CHRISTMAS);
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
