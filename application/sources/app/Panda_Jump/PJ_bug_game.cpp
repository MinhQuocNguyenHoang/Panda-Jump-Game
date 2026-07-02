#include "PJ_bug_game.h"

bug_t bug;

void spawn_bug(bug_t *bug, int16_t x, int16_t y, int16_t speed, bool direction)
{
    bug->x = x;
    bug->y = y;
    bug->speed = speed;
    bug->direction = direction;
}

void pj_bugs_game_handler(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case PJ_BUG_GAME_SETUP:
    {
        APP_DBG_SIG("Bug game setup\n");
        spawn_bug(&bug, 0, 32, 1, true); // Spawn bug at left side of screen
        task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUG_GAME_UPDATE);
    }
    break;

    case PJ_BUG_GAME_UPDATE:
    {
        // Update bug position based on its speed and direction
        if (bug.direction)
        {
            bug.x += bug.speed;
            if (bug.x > AXIS_X_PANDA_MAX)
            {
                bug.direction = false; // Change direction to left
            }
        }
        else
        {
            bug.x -= bug.speed;
            if (bug.x < 0)
            {
                bug.direction = true; // Change direction to right
            }
        }

        task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUG_GAME_UPDATE);
    }
    break;

    default:
        break;
    }
}