#include "scr_panda_game.h"
#include "PJ_panda_game.h"

static void view_scr_panda_game();

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

void pj_game_frame_display() {
  for (int i = 16; i < 89; i += 24) {
    view_render.drawBitmap(i, 0, bamboo, 8, 64, WHITE);
  }
}

void pj_panda_display() {
  if ((panda.x / 24) % 2 == 0) {
    view_render.drawBitmap(panda.x, panda.y, panda_left, 16, 16, WHITE);
  } else {
    view_render.drawBitmap(panda.x, panda.y, panda_right, 16, 16, WHITE);
  }
}

static void view_scr_panda_game() {
  pj_game_frame_display();
  pj_panda_display();
}

void scr_panda_game_handle(ak_msg_t *msg) {
  switch (msg->sig) {
  case SCREEN_ENTRY: {
    APP_DBG_SIG("PANDA SCREEN_ENTRY\n");
    BUZZER_PlaySound(BUZZER_SOUND_LETS_GO);
    timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);
    task_post_pure_msg(PJ_PANDA_GAME_ID, PJ_PANDA_GAME_SETUP);
    timer_set(PJ_PANDA_GAME_ID, PJ_GAME_TIME_TICK, PJ_GAME_TIME_TICK_INTERVAL,
              TIMER_PERIODIC);
  } break;

  case AC_DISPLAY_PANDA_GAME_UPDATE: {

  } break;

  case AC_DISPLAY_BUTON_UP_PRESSED: {
    APP_DBG_SIG("PANDA UP\n");
    task_post_pure_msg(PJ_PANDA_GAME_ID, PJ_PANDA_GAME_UP);
  } break;

  case AC_DISPLAY_BUTON_DOWN_PRESSED: {
    APP_DBG_SIG("PANDA DOWN\n");
    task_post_pure_msg(PJ_PANDA_GAME_ID, PJ_PANDA_GAME_DOWN);
  } break;

  case AC_DISPLAY_BUTON_MODE_PRESSED: { // note spelling: BUTON
    APP_DBG_SIG("PANDA EXIT\n");
    timer_remove_attr(PJ_PANDA_GAME_ID, PJ_GAME_TIME_TICK);
    SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
  } break;

  default:
    break;
  }
}
