#include "pj_arrow_game.h"

arrow_t arrows[MAX_ARROWS];
void spawn_arrow(arrow_t *arrow, int16_t x, int16_t y, int16_t speed)
{
    arrow->x = x;
    arrow->y = y;
    arrow->speed = speed;
    arrow->active = true;
}

void arrow_update()
{
    // Move and update active arrows
    for (int i = 0; i < MAX_ARROWS; i++)
    {
        if (arrows[i].active)
        {
            arrows[i].x += arrows[i].speed;
            if (arrows[i].x > 102)
            {
                arrows[i].active = false;
            }
        }
    }

    uint8_t max_arrows = 1;
    uint8_t spawn_prob = 100;
    int16_t speed_min = 1;
    int16_t speed_max = 1;

    switch (game_settings.difficulty)
    {
    case 0:
        max_arrows = 1;
        spawn_prob = 3; // 3% chance every 100ms (~3.3 seconds average cooldown)
        speed_min = 1;
        speed_max = 1;
        break;
    case 1:
        max_arrows = 1;
        spawn_prob = 6; // 6% chance every 100ms (~1.6 seconds average cooldown)
        speed_min = 1;
        speed_max = 2;
        break;
    case 2:
        max_arrows = 2;
        spawn_prob = 10; // 10% chance every 100ms (1 second average cooldown)
        speed_min = 2;
        speed_max = 3;
        break;
    default:
        break;
    }

    // Count active arrows
    uint8_t active_count = 0;
    for (int i = 0; i < MAX_ARROWS; i++)
    {
        if (arrows[i].active)
        {
            active_count++;
        }
    }

    if (active_count < max_arrows && (rand() % 100) < spawn_prob)
    {
        for (int i = 0; i < MAX_ARROWS; i++)
        {
            if (!arrows[i].active)
            {
                int16_t speed = speed_min + (rand() % (speed_max - speed_min + 1));
                spawn_arrow(&arrows[i], 0, rand() % 64, speed);
                break;
            }
        }
    }
}

void pj_arrow_game_handler(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case PJ_ARROW_GAME_SETUP:
    {
        for (int i = 0; i < MAX_ARROWS; i++)
        {
            arrows[i].active = false;
        }
    }
    break;

    case PJ_ARROW_GAME_UPDATE:
    {
        arrow_update();
    }
    break;

    default:
        break;
    }
}