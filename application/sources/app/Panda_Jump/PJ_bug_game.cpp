#include "PJ_bug_game.h"
#include <stdlib.h>

bug_t bugs[MAX_BUGS];

// Coordinates of the 3 bamboo trunks
static const uint8_t PANDA_BAMBOO_X[] = {16, 48, 80};

void spawn_bug(bug_t *bug, int16_t x, int16_t y, int16_t speed, bool direction, bool side)
{
    bug->x = x;
    bug->y = y;
    bug->speed = speed;
    bug->direction = direction;
    bug->side = side;
    bug->active = true;
    bug->explosion_ticks = 0;
}

void bug_update()
{
    // Get configurations based on difficulty
    uint8_t max_bugs = 1;
    uint8_t spawn_prob = 100;
    int16_t speed_min = 1;
    int16_t speed_max = 1;

    switch (game_settings.difficulty)
    {
    case 0: // EASY
        max_bugs = 2;
        spawn_prob = 20;
        speed_min = 1;
        speed_max = 1;
        break;
    case 1: // MEDIUM
        max_bugs = 3;
        spawn_prob = 40;
        speed_min = 1;
        speed_max = 2;
        break;
    case 2: // HARD
    default:
        max_bugs = 5;
        spawn_prob = 60;
        speed_min = 2;
        speed_max = 3;
        break;
    }

    // Update position of all active bugs (vertical movement)
    for (int i = 0; i < MAX_BUGS; i++)
    {
        if (bugs[i].active)
        {
            if (bugs[i].explosion_ticks > 0)
            {
                bugs[i].explosion_ticks--;
                if (bugs[i].explosion_ticks == 0)
                {
                    bugs[i].active = false; // Deactivate after explosion finishes
                }
            }
            else
            {
                if (bugs[i].direction) // true = crawls down
                {
                    bugs[i].y += bugs[i].speed;
                    if (bugs[i].y > 64) // Off-screen bottom
                    {
                        bugs[i].active = false;
                    }
                }
                else // false = crawls up
                {
                    bugs[i].y -= bugs[i].speed;
                    if (bugs[i].y < -8) // Off-screen top
                    {
                        bugs[i].active = false;
                    }
                }
            }
        }
    }

    // Count active bugs
    uint8_t active_count = 0;
    for (int i = 0; i < MAX_BUGS; i++)
    {
        if (bugs[i].active)
        {
            active_count++;
        }
    }

    // Try to spawn a new bug if active count is below limit
    if (active_count < max_bugs && (rand() % 100) < spawn_prob)
    {
        for (int i = 0; i < MAX_BUGS; i++)
        {
            if (!bugs[i].active)
            {
                // Random vertical direction: true = down, false = up
                bool dir = (rand() % 2 == 0);
                int16_t start_y = dir ? -8 : 64;

                // Random bamboo lane (0, 1, or 2)
                uint8_t lane = rand() % 3;

                // Random side of the bamboo: false = left, true = right
                bool side = (rand() % 2 == 0);

                // Compute X coordinate to snap the bug to the trunk
                int16_t start_x = side ? (PANDA_BAMBOO_X[lane] + 8) : (PANDA_BAMBOO_X[lane] - 8);

                // Random speed based on difficulty limits
                int16_t speed = speed_min + (rand() % (speed_max - speed_min + 1));

                spawn_bug(&bugs[i], start_x, start_y, speed, dir, side);
                break;
            }
        }
    }
}

void pj_bugs_game_handler(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case PJ_BUG_GAME_SETUP:
    {
        APP_DBG_SIG("Bug game setup\n");
        // Clear all bugs
        for (int i = 0; i < MAX_BUGS; i++)
        {
            bugs[i].active = false;
            bugs[i].explosion_ticks = 0;
        }
        // Spawn the first bug: Lane 1 (middle), left side, moving down, speed 1, starting at y=0
        spawn_bug(&bugs[0], PANDA_BAMBOO_X[1] - 8, 0, 1, true, false);
        task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUG_GAME_UPDATE);
    }
    break;

    case PJ_BUG_GAME_UPDATE:
    {
        bug_update();
        task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUG_GAME_UPDATE);
    }
    break;

    default:
        break;
    }
}