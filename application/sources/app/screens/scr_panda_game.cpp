#include "scr_panda_game.h"

uint8_t ar_game_state = GAME_OFF;

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

  // Shifted divider line to 102 (was 81)
  view_render.drawFastVLine(102, 0, 64, WHITE);

  // Shifted score box to 104 (was 84) with width 22 (was 30)
  view_render.drawRoundRect(104, 2, 22, 27, 3, WHITE);

  // Draw a mini star icon shifted by +16px: center is 114 (was 98)
  view_render.drawPixel(114, 5, WHITE);
  view_render.drawLine(112, 6, 116, 6, WHITE);
  view_render.drawLine(111, 7, 117, 7, WHITE);
  view_render.drawLine(113, 8, 115, 8, WHITE);
  view_render.drawPixel(112, 9, WHITE);
  view_render.drawPixel(116, 9, WHITE);

  // Print score (centered in the box)
  view_render.setTextSize(1);
  view_render.setTextColor(WHITE);
  if (panda.score < 10)
  {
    view_render.setCursor(112, 16);
  }
  else if (panda.score < 100)
  {
    view_render.setCursor(109, 16);
  }
  else
  {
    view_render.setCursor(106, 16);
  }
  view_render.print(panda.score);

  // --- TIME BOX (BOTTOM RIGHT) ---
  // Draw rounded box for survival time shifted by +16px: 104 (was 84)
  view_render.drawRoundRect(104, 33, 22, 28, 3, WHITE);

  // Draw a mini clock icon centered at 114 (was 98)
  view_render.drawCircle(114, 40, 4, WHITE);
  view_render.drawLine(114, 40, 114, 38, WHITE); // Hour hand
  view_render.drawLine(114, 40, 116, 40, WHITE); // Minute hand

  // Calculate and print survival time in seconds
  uint32_t seconds = panda.survival_time_ticks / 2;
  if (seconds < 10)
  {
    view_render.setCursor(110, 48);
  }
  else if (seconds < 100)
  {
    view_render.setCursor(107, 48);
  }
  else
  {
    view_render.setCursor(104, 48);
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

static bool check_collision(int16_t px, int16_t py, int16_t bx, int16_t by)
{
  return (px < bx + 8 && px + 16 > bx &&
          py < by + 8 && py + 16 > by);
}

void pj_bug_display(bug_t *bug)
{
  if (bug->direction) // true = crawls down
  {
    if (bug->side) // true = right side of bamboo
    {
      view_render.drawBitmap(bug->x, bug->y, bug_right_down, 8, 8, WHITE);
    }
    else // false = left side of bamboo
    {
      view_render.drawBitmap(bug->x, bug->y, bug_left_down, 8, 8, WHITE);
    }
  }
  else // false = crawls up
  {
    if (bug->side) // true = right side of bamboo
    {
      view_render.drawBitmap(bug->x, bug->y, bug_right, 8, 10, WHITE);
    }
    else // false = left side of bamboo
    {
      view_render.drawBitmap(bug->x, bug->y, bug_left, 8, 10, WHITE);
    }
  }
}

static void view_scr_panda_game()
{
  view_render.clear();

  if (ar_game_state == GAME_PLAY)
  {
    // Draw falling/blowing leaves (leaf effect)
    view_render.drawPixel((128 - (tick_count * 2)) % 138 - 10, 15, WHITE);
    view_render.drawPixel((128 - (tick_count * 3)) % 138 - 10, 35, WHITE);
    view_render.drawPixel((128 - (tick_count * 1)) % 138 - 10, 48, WHITE);

    pj_game_frame_display();
    pj_panda_display();

    for (int i = 0; i < MAX_BUGS; i++)
    {
      if (bugs[i].active)
      {
        if (bugs[i].explosion_ticks > 0)
        {
          // Center the 16x10 boom bitmap over the 8x8 bug
          view_render.drawBitmap(bugs[i].x - 4, bugs[i].y - 1, boom, 16, 10, WHITE);
        }
        else
        {
          pj_bug_display(&bugs[i]);
        }
      }
    }
  }
  else if (ar_game_state == GAME_OVER)
  {
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);

    view_render.setCursor(35, 10);
    view_render.print("GAME OVER");

    view_render.setCursor(20, 25);
    view_render.print("Score: ");
    view_render.print(panda.score);

    view_render.setCursor(20, 35);
    view_render.print("Time: ");
    view_render.print(panda.survival_time_ticks / 2);
    view_render.print("s");

    view_render.setCursor(2, 52);
    view_render.print("UP:Retry MODE:Exit");
  }
}

void scr_panda_game_handle(ak_msg_t *msg)
{
  switch (msg->sig)
  {
  case SCREEN_ENTRY:
  {
    APP_DBG_SIG("PANDA SCREEN_ENTRY\n");
    ar_game_state = GAME_PLAY;
    if (game_settings.sound_en)
    {
      BUZZER_PlaySound(BUZZER_SOUND_LETS_GO);
    }
    tick_count = 0;
    timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);
    task_post_pure_msg(PJ_PANDA_GAME_ID, PJ_PANDA_GAME_SETUP);
    task_post_pure_msg(PJ_BUG_GAME_ID, PJ_BUG_GAME_SETUP);
    timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_PANDA_GAME_UPDATE, 100,
              TIMER_PERIODIC);
  }
  break;

  case AC_DISPLAY_PANDA_GAME_UPDATE:
  {
    tick_count++;

    // Perform collision check on display ticks (for higher responsiveness)
    if (ar_game_state == GAME_PLAY)
    {
      task_post_pure_msg(PJ_BUG_GAME_ID, PJ_BUG_GAME_UPDATE);

      // Increment survival time ticks every 5 display ticks (5 * 100ms = 500ms)
      if (tick_count % 5 == 0)
      {
        panda.survival_time_ticks++;
      }

      uint8_t bx = BAMBOO_X[panda.lane];
      uint8_t px = (panda.side == 0) ? (bx - 12) : (bx + 4);
      for (int i = 0; i < MAX_BUGS; i++)
      {
        if (bugs[i].active && bugs[i].explosion_ticks == 0 && check_collision(px, panda.y, bugs[i].x, bugs[i].y))
        {
          if (panda.y < bugs[i].y) // Panda is physically higher: stomp the bug!
          {
            bugs[i].explosion_ticks = 4; // Start explosion
            if (game_settings.sound_en)
            {
              BUZZER_PlaySound(BUZZER_SOUND_CLICK);
            }
            uint8_t pts = 10;
            if (game_settings.difficulty == 1)
            {
              pts = 20;
            }
            else if (game_settings.difficulty == 2)
            {
              pts = 30;
            }
            panda.score += pts;
          }
          else // Collision from below or level: Game Over!
          {
            ar_game_state = GAME_OVER;
            if (game_settings.sound_en)
            {
              BUZZER_PlaySound(BUZZER_SOUND_BANG);
            }
            break;
          }
        }
      }
    }
  }
  break;

  case AC_DISPLAY_BUTON_UP_PRESSED:
  {
    APP_DBG_SIG("PANDA LEFT\n");
    if (ar_game_state == GAME_PLAY)
    {
      task_post_pure_msg(PJ_PANDA_GAME_ID, PJ_PANDA_JUMP_RIGHT);
    }
    else if (ar_game_state == GAME_OVER)
    {
      ar_game_state = GAME_PLAY;
      task_post_pure_msg(PJ_PANDA_GAME_ID, PJ_PANDA_GAME_SETUP);
      task_post_pure_msg(PJ_BUG_GAME_ID, PJ_BUG_GAME_SETUP);
    }
  }
  break;

  case AC_DISPLAY_BUTON_DOWN_PRESSED:
  {
    APP_DBG_SIG("PANDA RIGHT\n");
    if (ar_game_state == GAME_PLAY)
    {
      task_post_pure_msg(PJ_PANDA_GAME_ID, PJ_PANDA_JUMP_LEFT);
    }
  }
  break;

  case AC_DISPLAY_BUTON_MODE_RELEASED:
  {
    APP_DBG_SIG("PANDA UP\n");
    if (ar_game_state == GAME_PLAY)
    {
      task_post_pure_msg(PJ_PANDA_GAME_ID, PJ_PANDA_GAME_UP);
    }
    else if (ar_game_state == GAME_OVER)
    {
      timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_PANDA_GAME_UPDATE);
      SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
    }
  }
  break;

  case AC_DISPLAY_BUTON_MODE_LONG_RELEASED:
  {
    APP_DBG_SIG("PANDA DOWN\n");
    if (ar_game_state == GAME_PLAY)
    {
      task_post_pure_msg(PJ_PANDA_GAME_ID, PJ_PANDA_GAME_DOWN);
    }
  }
  break;

  default:
    break;
  }
}
