#include "PJ_panda_game.h"
#include "scr_panda_game.h"

PJ_PANDA_t panda;

void pj_panda_game_handler(ak_msg_t *msg)
{
  switch (msg->sig)
  {
  case PJ_PANDA_GAME_SETUP:
  {
    APP_DBG_SIG("Panda game setup\n");
    panda.lane = 1; /* Start on middle bamboo */
    panda.side = 0; /* Start on Left side of the bamboo */
    panda.y = AXIS_Y_PANDA;
    panda.score = 0;
    panda.survival_time_ticks = 0;
    task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_PANDA_GAME_UPDATE);
  }
  break;

  case PJ_PANDA_GAME_UP:
  {
    if (ar_game_state == GAME_PLAY)
    {
      if (panda.y <= 0)
      {
        panda.y = 0;
      }
      else
      {
        panda.y -= 4; // Move up by 4 pixels
      }
    }
    task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_PANDA_GAME_UPDATE);
  }
  break;

  case PJ_PANDA_GAME_DOWN:
  {
    if (ar_game_state == GAME_PLAY)
    {
      if (panda.y >= AXIS_Y_PANDA_MAX)
      {
        panda.y = AXIS_Y_PANDA_MAX;
      }
      else
      {
        panda.y += 4; // Move down by 4 pixels
      }
    }
    task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_PANDA_GAME_UPDATE);
  }
  break;

  case PJ_PANDA_JUMP_LEFT:
  {
    if (ar_game_state == GAME_PLAY)
    {
      if (panda.side == 1)
      {                 // On right side of bamboo
        panda.side = 0; // Jump to left side of same bamboo
      }
      else
      { // On left side of bamboo
        if (panda.lane > 0)
        {
          panda.lane--;
          panda.side = 1; // Land on right side of left bamboo
        }
      }
    }
    task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_PANDA_GAME_UPDATE);
  }
  break;

  case PJ_PANDA_JUMP_RIGHT:
  {
    if (ar_game_state == GAME_PLAY)
    {
      if (panda.side == 0)
      {                 // On left side of bamboo
        panda.side = 1; // Jump to right side of same bamboo
      }
      else
      { // On right side of bamboo
        if (panda.lane < 2)
        {
          panda.lane++;
          panda.side = 0; // Land on left side of right bamboo
        }
      }
    }
    task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_PANDA_GAME_UPDATE);
  }
  break;



  default:
    break;
  }
}
