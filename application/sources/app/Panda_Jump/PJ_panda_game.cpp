#include "PJ_panda_game.h"

PJ_PANDA_t panda;

#define AXIS_Y_PANDA_MAX (48)
#define AXIS_X_PANDA_MAX (127)
#define AXIS_X_PANDA_JUMP_MAX (111)

void pj_panda_game_handler(ak_msg_t *msg) {
  switch (msg->sig) {
  case PJ_PANDA_GAME_SETUP: {
    APP_DBG_SIG("Panda game setup\n");
    panda.x = AXIS_X_PANDA;
    panda.y = AXIS_Y_PANDA;
    task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_PANDA_GAME_UPDATE);
  } break;

  case PJ_PANDA_GAME_UP: {
    if (panda.y <= 0) {
      panda.y = 0;
    } else {
      panda.y -= 4; // Move up by 4 pixels
    }
    task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_PANDA_GAME_UPDATE);
  } break;

  case PJ_PANDA_GAME_DOWN: {
    if (panda.y >= AXIS_Y_PANDA_MAX) {
      panda.y = AXIS_Y_PANDA_MAX;
    } else {
      panda.y += 4; // Move down by 4 pixels
    }
    task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_PANDA_GAME_UPDATE);
  } break;

  case PJ_PANDA_JUMP_LEFT: {
    if (panda.x >= 111) {
      panda.x = 111;
    } else {
      panda.x += 24;
      // Climbing up effect on jump
      if (panda.y > 6) {
        panda.y -= 8;
      } else {
        panda.y = 6;
      }
    }
    task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_PANDA_GAME_UPDATE);
  } break;

  case PJ_PANDA_JUMP_RIGHT: {
    if (panda.x <= 15) {
      panda.x = 15;
    } else {
      panda.x -= 24;
      // Climbing up effect on jump
      if (panda.y > 6) {
        panda.y -= 8;
      } else {
        panda.y = 6;
      }
    }
    task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_PANDA_GAME_UPDATE);
  } break;

  default:
    break;
  }
}
